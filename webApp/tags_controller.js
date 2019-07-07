
app.controller('edit_tags', function ($scope, $http, $rootScope, host) {
	$scope.tree = null;

	$scope.load_tags = function () {
		
		$http({
			url: host + "edit_tags.php",

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

		}, function (response) {
			$rootScope.event_log.error( read_error_msg(response) );
		});
	}

	$scope.send_add_request = function (tag_name_, parent_id_) {
		const params_object = {
			tag_name: tag_name_,
			parent_id: parent_id_
		};

		$http({
			url: host + "add_tag.php",

			method: "GET",
			params: {
				date: new Date().getTime(),
				content: JSON.stringify(params_object)
			}
		}).then(function (response) {
			$rootScope.event_log.success(response.data.message);
			$scope.load_tags();
		}, function (response) {
			$rootScope.event_log.error("Cannot add '" + tag_name + "' tag");
		});
	}

	$scope.send_delete_request = function (tag_id_, tag_name_) {
		const params_object = {
			tag_id: tag_id_,
		};

		$http({
			url: host + "delete_tag.php",
			method: "GET",
			params: {
				date: new Date().getTime(),
				content: JSON.stringify(params_object)
			}
		}).then(function (response) {
			$rootScope.event_log.success(response.data.message);
			$scope.load_tags();
		}, function (response) {
			$rootScope.event_log.error("Cannot delete '" + tag_name_ + "' tag");
		});
	}

	$scope.send_update_request = function (tag_id_, tag_name_) {
		const params_object = {
			tag_id: tag_id_,
			tag_name: tag_name_
		};

		$http({
			url: host + "update_tag.php",
			method: "GET",
			params: {
				date: new Date().getTime(),
				content: JSON.stringify(params_object)
			}
		}).then(function (response) {
			$rootScope.event_log.success(response.data.message);
			$scope.load_tags();
		}, function (response) {
			$rootScope.event_log.error("Cannot update '" + tag_name_ + "' tag");
		});
	}

	$scope.add_tag = function () {
		if ($scope.tree == null) {
			$rootScope.event_log.error("Tree is not loaded");
			return;
		}

		let tag_name = read_from_input("add_tag_name_input");
		let parent_name = read_from_input("add_tag_parent_input");
		
		if (tag_name == "") {
			$rootScope.event_log.error("Empty tag field");
			return;
		}

		if ($scope.tree.has_tag(tag_name)) {
			$rootScope.event_log.error("Tag '" + tag_name + "' already exists");
			return;
		}
		
		if (parent_name != "" && !$scope.tree.has_tag(parent_name)) {
			$rootScope.event_log.error("There is no such tag as '" + parent_name + "'");
			return;
		}
		
		let parent_id = $scope.tree.get_tag_id(parent_name);

		$scope.send_add_request(tag_name, parent_id);
	}

	$scope.delete_tag = function () {
		if ($scope.tree == null) {
			$rootScope.event_log.error("Tree is not loaded");
			return;
		}

		let tag_name = read_from_input("delete_tag_name_input");

		if (tag_name == "") {
			$rootScope.event_log.error("Empty tag field");
			return;
		}

		if (!$scope.tree.has_tag(tag_name)) {
			$rootScope.event_log.error("Tag '" + tag_name + "' does not exist");
			return;
		}

		let tag_id = $scope.tree.get_tag_id(tag_name);
		$scope.send_delete_request(tag_id, tag_name);
	}

	$scope.update_tag = function () {
		if ($scope.tree == null) {
			$rootScope.event_log.error("Tree is not loaded");
			return;
		}

		const old_tag_name = read_from_input("update_tag_input_old");
		const new_tag_name = read_from_input("update_tag_input_new");

		if (old_tag_name == "" || new_tag_name == "") {
			$rootScope.event_log.error("Empty tag field");
			return;
		}

		if (!$scope.tree.has_tag(old_tag_name)) {
			$rootScope.event_log.error("Tag '" + old_tag_name + "' does not exist");
			return;
		}

		if ($scope.tree.has_tag(new_tag_name)) {
			$rootScope.event_log.error("Tag '" + new_tag_name + "' already exists");
			return;
		}


		let old_tag_id = $scope.tree.get_tag_id(old_tag_name);
		$scope.send_update_request(old_tag_id, new_tag_name);
	}
	
});
