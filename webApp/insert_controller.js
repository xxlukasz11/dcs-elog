function get_input_data() {
	let title_input_value = read_from_input('title_input');
	let description_input_value = read_from_input('description_input').replace(/\r?\n/g, '<br/>');
	let tags_input_value = read_lower_from_input('insert_tags_input');
	let author_input_value = read_from_input('author_input');

	return {
		title: title_input_value,
		description: description_input_value,
		tags: tags_input_value,
		author: author_input_value
	};
}


app.controller('insert_data', function ($scope, sender, logger) {
	$scope.send_insert_request = function () {

		sender.send("insert.php", get_input_data()).then(
		function (response) {
			logger.get_log().success( response.data.message );
		}, function (response) {
			logger.get_log().error(read_error_msg(response));
		});
	}
});
