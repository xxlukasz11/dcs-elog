
app.controller('edit_tags', function ($scope, $http) {
	$scope.load_tags = function () {
		
		$http({
			// remote
			//url: "http://lukboz.000webhostapp.com/insert.php",

			// localhost
			url: "edit_tags.php",

			method: "GET",
			params: {
				date: new Date().getTime(),
				content: "tags"
			}
		}).then(function (response) {
			let tree = from_table_to_tree(response.data);
			let container = document.getElementById('tags_tree_container');
			tree.set_container(container);
			tree.display();
			
			$scope.error_info = "";
		}, function (response) {
			$scope.error_info = read_error_msg(response);
		});
	}

	$scope.add_tag = function () {
		
	}
	
});
