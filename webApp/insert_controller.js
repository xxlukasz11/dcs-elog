function uint8array_to_binary_string(array) {
	const len = array.byteLength;
	let binaryString = "";
	for (let i = 0; i < len; i++) {
		binaryString += String.fromCharCode(array[i]);
	}
	return binaryString;
}

class Attachment {
	constructor(name, type, size) {
		this.name = name;
		this.type = type;
		this.size = size;

		this.loaded = false;
		this.payload = null;

		this.progress_value = 0;
		this.progress_max = 0;
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
		const attachment = new Attachment(file.name, file.type, file.size);
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

	$scope.pack_parameters = function () {
		const obj = {
			title: $scope.title,
			description: $scope.description,
			tags: $scope.tags.toLowerCase(),
			author: $scope.author,
			attachments: $scope.file_list
		};
		console.log(obj);
		return obj;
	}

	$scope.send_insert_request = function () {

		sender.send("insert.php", $scope.pack_parameters()).then(
		function (response) {
			logger.get_log().data(response.data);
		}, function (response) {
			logger.get_log().error(read_error_msg(response));
		});
	}
});
