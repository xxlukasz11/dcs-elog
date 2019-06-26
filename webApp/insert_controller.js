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


app.controller('insert_data', function ($scope, $http, $rootScope, host) {
	$scope.send_insert_request = function () {

		$http({
			url: host + "insert.php",

			method: "GET",
			params: {
				date: new Date().getTime(),
				content: JSON.stringify(get_input_data())
			}
		}).then(function (response) {
			$rootScope.event_log.success( response.data.message );
		}, function (response) {
			$rootScope.event_log.error( read_error_msg(response) );
		});
	}
});
