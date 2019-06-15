function get_input_data() {
    let title_input = document.getElementById('title_input');
    let description_input = document.getElementById('description_input');
    let tags_input = document.getElementById('insert_tags_input');
    let author_input = document.getElementById('author_input');

    return {
        title: title_input.value,
        description: description_input.value,
        tags: tags_input.value,
        author: author_input.value
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
            $scope.success_info = response.data.message;
            $scope.error_info = "";
        }, function (response) {
            $scope.success_info = "";
			$scope.error_info = read_error_msg(response);
        });
    }
});
