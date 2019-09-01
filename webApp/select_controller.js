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

	$scope.update_event = function(event, event_id) {
		const manager = new Update_event_manager(event.target);
		const parameter_object = {
			id: event_id,
			title: manager.get_title_from_input(),
			tags: manager.get_tags_from_input(),
			description: manager.get_description_from_input()
		}
		logger.get_console().log_message("UPDATE EVENT", parameter_object)
		manager.display_mode();
		//$scope.send_update_request(parameter_object);
	}

	$scope.send_update_request = function (parameter_object) {
		sender.send("update_event.php", parameter_object).then(
		function (response) {
			logger.get_log().event("Event updated");
		}, function (response) {
			logger.get_log().error(read_error_msg(response));
		});
	}

    $scope.send_request = function () {
    	sender.send("select.php", getParams()).then(
		function (response) {
        	$scope.table_data = response.data;
        	logger.get_log().event("Events loaded");
        }, function (response) {
        	logger.get_log().error(read_error_msg(response));
        });
    }

    $scope.sort_order = false;

    $scope.order_by_th = function (th) {
        $scope.active_th = th;
        $scope.sort_order = !$scope.sort_order;
    }

});
