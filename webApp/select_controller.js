function getParams() {
    let min_date_input = document.getElementById('min_date_input');
    let max_date_input = document.getElementById('max_date_input');
    let tags_input = document.getElementById('tags_input');

    return {
        min_date: min_date_input.value,
        max_date: max_date_input.value,
        tags: tags_input.value
    };
}

app.controller('select_data', function ($scope, $http, $rootScope) {
    $scope.send_request = function () {

        $http({
            // remote
            //url: "http://lukboz.000webhostapp.com/select.php",

            // localhost
            url: "select.php",

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