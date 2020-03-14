function getParams() {
    let min_date_input = document.getElementById('min_date_input');
    let max_date_input = document.getElementById('max_date_input');
    let tags_input_value = read_lower_from_input('tags_input');

    return {
        min_date: min_date_input.value,
        max_date: max_date_input.value,
        tags: tags_input_value
    };
}

app.controller('select_data', function ($scope, sender, logger) {

	$scope.toggle_hidden = function (element) {
		element.classList.toggle('hidden');
	}

	$scope.toggle_description = function (event) {
		const element = event.target.parentElement.parentElement.nextElementSibling;
		$scope.toggle_hidden(element)
	}

	$scope.show_edit_view = function(event) {
		const manager = new Update_event_manager(event.target);
		manager.edit_mode();
	}

	$scope.close_edit_view = function (event, data) {
		const manager = new Update_event_manager(event.target);
		manager.display_mode();
	}

	$scope.is_image_type = function (type) {
		return type.includes('image');
	}

	$scope.disable_bubble_event = function($event) {
		if ($event.stopPropagation) $event.stopPropagation();
		if ($event.preventDefault) $event.preventDefault();
		$event.cancelBubble = true;
		$event.returnValue = false;
	}

	$scope.remove_attachment = function ($event, event_id, attachment_id) {
		$scope.disable_bubble_event($event);
		console.log("Event id: " + event_id + " Attachment id: " + attachment_id);
		// TODO
	}

	$scope.open_attachment_in_new_tab = function (payload, type) {
		const byteCharacters = atob(payload);
		const byteNumbers = new Array(byteCharacters.length);
		for (var i = 0; i < byteCharacters.length; i++) {
			byteNumbers[i] = byteCharacters.charCodeAt(i);
		}
		var byteArray = new Uint8Array(byteNumbers);
		var blob = new Blob([byteArray], { type: type + ';base64' });
		var url = URL.createObjectURL(blob);
		window.open(url);
	}

	$scope.update_event = function (event, event_id) {
		const button_handler = new Button_load_handler(event.target);
		button_handler.animate();

		const manager = new Update_event_manager(event.target);
		const parameter_object = {
			id: event_id,
			title: manager.get_title_from_input(),
			tags: manager.get_tags_from_input(),
			description: encode_line_breaks(manager.get_description_from_input())
		}
		$scope.send_update_request(parameter_object, button_handler, manager);
	}

	$scope.send_update_request = function (parameter_object, button_handler, update_event_manager) {
		sender.send("update_event.php", parameter_object).then(
		function (response) {
			update_event_manager.display_mode();
			$scope.trigger_load_events();
			logger.get_log().event(response.data.message);
			button_handler.reset();
		}, function (response) {
			logger.get_log().error(read_error_msg(response));
			button_handler.reset();
		});
	}

	$scope.load_events = function ($event) {
		const button_handler = new Button_load_handler($event.target);
		button_handler.animate();
		$scope.send_select_request(button_handler);
	}

	$scope.trigger_load_events = function () {
		const button_handler = new Button_load_handler();
		$scope.send_select_request(button_handler);
	}

	$scope.send_select_request = function (button_handler) {
    	sender.send("select.php", getParams()).then(
		function (response) {
			const response_data = response.data;
			logger.get_log().data(response_data);
			$scope.table_data = response_data.data;
			button_handler.reset();
        }, function (response) {
        	logger.get_log().error(read_error_msg(response));
        	button_handler.reset();
        });
	}

    $scope.sort_order = false;

    $scope.order_by_th = function (th) {
        $scope.active_th = th;
        $scope.sort_order = !$scope.sort_order;
    }

});
