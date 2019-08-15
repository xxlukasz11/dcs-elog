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

app.controller('select_data', function ($scope, $http, $rootScope, host) {

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
		console.log("=== EVENT UPDATE ===");
		console.log("Event id: " + event_id);
		console.log("New title: " + manager.get_title_from_input());
		console.log("New tags: " + manager.get_tags_from_input());
		console.log("New description: " + manager.get_description_from_input());
	}

    $scope.send_request = function () {

        $http({
            url: host + "select.php",
            method: "GET",
            params: {
                date: new Date().getTime(),
                content: JSON.stringify(getParams())
            }
        }).then(function (response) {
        	$scope.table_data = response.data;
        	$rootScope.event_log.success("Events loaded");
        }, function (response) {
        	$rootScope.event_log.error( read_error_msg(response) );
        });
    }

    $scope.sort_order = false;

    $scope.order_by_th = function (th) {
        $scope.active_th = th;
        $scope.sort_order = !$scope.sort_order;
    }

});