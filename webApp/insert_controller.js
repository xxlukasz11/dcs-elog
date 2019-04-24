function get_input_data() {
    let title_input = document.getElementById('title_input');
    let description_input = document.getElementById('description_input');
    let tags_input = document.getElementById('insert_tags_input');

    return {
        title: title_input.value,
        description: description_input.value,
        tags: tags_input.value
    };
}


app.controller('insert_data', function ($scope, $http) {
    $scope.send_insert_request = function () {

        $http({
            // remote
            //url: "http://lukboz.000webhostapp.com/insert.php",

            // localhost
            url: "insert.php",

            method: "GET",
            params: {
                date: new Date().getTime(),
                content: JSON.stringify(get_input_data())
            }
        }).then(function (response) {
            $scope.success_info = "Success!"
            $scope.error_info = "";
        }, function (res) {
            $scope.success_info = "";

            let error_msg = res.data.message;
            if (error_msg)
                $scope.error_info = res.data.message;
            else
                $scope.error_info = standard_error_message;
        });
    }
});
