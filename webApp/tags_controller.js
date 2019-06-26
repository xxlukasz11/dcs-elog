
app.controller('edit_tags', function ($scope, $http) {
	$scope.tree = null;

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
			$scope.tree = from_table_to_tree(response.data);
			let container = document.getElementById('tags_tree_container');
			$scope.tree.set_container(container);
			$scope.tree.display();
			
			$scope.error_info = "";
		}, function (response) {
			$scope.error_info = read_error_msg(response);
		});
	}

	$scope.add_tag = function () {
		if ($scope.tree == null) {
			$scope.error_info = "There is no tree loaded";
			return;
		}

		let tag_name = document.getElementById("add_tag_name_input").value.toLowerCase();
		let parent_name = document.getElementById("add_tag_parent_input").value.toLowerCase();
		
		if ($scope.tree.has_tag(tag_name)) {
			$scope.error_info = "Tag '" + tag_name + "' already exists";
			return;
		}
		
		if (parent_name != "" && !$scope.tree.has_tag(parent_name)) {
			$scope.error_info = "There is no such tag as '" + parent_name + "'";
			return;
		}
		
		$scope.tree.add(tag_name, parent_name);
		$scope.tree.display();
	}
	
});
