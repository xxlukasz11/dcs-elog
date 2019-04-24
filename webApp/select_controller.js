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

app.controller('select_data', function ($scope, $http) {
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
            $scope.error_info = "";
        }, function (res) {
			let error_msg = res.data.message;
			if(error_msg)
				$scope.error_info = res.data.message;
			else
				$scope.error_info = "Can't load data. Server may be unavialiable.";
        });
    }

    $scope.sort_order = false;

    $scope.order_by_th = function (th) {
        $scope.active_th = th;
        $scope.sort_order = !$scope.sort_order;
    }

});