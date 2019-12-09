function uint8array_to_binary_string(array) {
	const len = array.byteLength;
	let binaryString = "";
	for (let i = 0; i < len; i++) {
		binaryString += String.fromCharCode(array[i]);
	}
	return binaryString;
}

class Attachment_item {
	constructor(name, type) {
		this.name = name;
		this.type = type;

		this.loaded = false;
		this.payload = null;

		this.progress_value = 0;
		this.progress_max = 0;
	}
}

class Attachment {
	constructor(name, type, payload) {
		this.name = name;
		this.type = type;
		this.payload = payload;

		if (!type) {
			this.type = "text/plain";
		}
	}
}

app.controller('insert_data', function ($scope, sender, logger) {

	$scope.title = "";
	$scope.description = "";
	$scope.tags = "";
	$scope.author = "";
	$scope.file_list = [];

	$scope.append_file = function () {
		const file_input = document.createElement('input');
		file_input.type = "file";
		file_input.onchange = $scope.load_file;
		file_input.click();
	}

	$scope.load_file = function () {
		const file = this.files[0];
		const attachment = new Attachment_item(file.name, file.type);
		$scope.file_list.push(attachment);
		$scope.$apply();

		const reader = new FileReader();
		reader.onprogress = function (e) {
			if (e.lengthComputable) {
				attachment.progress_max = e.total;
				attachment.progress_value = e.loaded;
				$scope.$apply();
			}
		}

		reader.onerror = function (event) {
			reader.abort();
			logger.get_log().error("Failed to load " + attachment.name + ". " + reader.error);
		};

		reader.onload = function () {
			const arrayBuffer = this.result;
			const array = new Uint8Array(arrayBuffer);
			const binaryString = uint8array_to_binary_string(array);
			const base64 = btoa(binaryString);

			attachment.payload = base64;
			attachment.loaded = true;
			$scope.$apply();
		}
		reader.readAsArrayBuffer(file);
	}

	$scope.remove_file = function (index) {
		$scope.file_list.splice(index, 1);
	}

	$scope.reset_fields = function () {
		$scope.title = "";
		$scope.description = "";
		$scope.tags = "";
		$scope.author = "";
		$scope.file_list = [];
	}

	$scope.check_all_files_loaded = function () {
		for (file of $scope.file_list) {
			if (!file.loaded) {
				return false;
			}
		}
		return true;
	}

	$scope.create_attachment_list = function () {
		let attachment_list = [];
		for (a of $scope.file_list) {
			attachment_list.push(new Attachment(
				a.name,
				a.type,
				a.payload
			));
		}
		return attachment_list;
	}

	$scope.pack_parameters = function () {
		return {
			title: $scope.title,
			description: encode_line_breaks($scope.description),
			tags: $scope.tags.toLowerCase(),
			author: $scope.author,
			attachments: $scope.create_attachment_list()
		};
	}

	$scope.send_insert_request = function ($event) {
		if (!$scope.check_all_files_loaded()) {
			logger.get_log().error("Cannot add event. Somme attachments are still loading.");
			return;
		}

		const button_handler = new Button_load_handler($event.target);
		button_handler.animate();

		sender.send("insert.php", $scope.pack_parameters()).then(
		function (response) {
			logger.get_log().data(response.data);
			$scope.reset_fields();
			button_handler.reset();
		}, function (response) {
			logger.get_log().error(read_error_msg(response));
			button_handler.reset();
		});
	}
});
