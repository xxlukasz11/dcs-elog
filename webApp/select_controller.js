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

	$scope.toggle_description = function (event) {
		event.target.parentElement.parentElement.nextElementSibling.classList.toggle('hidden');
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