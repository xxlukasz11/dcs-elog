
app.controller('edit_tags', function ($scope, sender, logger) {
	$scope.tree = null;

	$scope.load_tags = function () {
		
		sender.send("load_tags.php", {}).then(
		function (response) {
			$scope.tree = from_table_to_tree(response.data);
			let container = document.getElementById('tags_tree_container');
			$scope.tree.set_container(container);
			$scope.tree.display();
			logger.get_log().event("Tags have been loaded");

		}, function (response) {
			logger.get_log().error(read_error_msg(response));
		});
	}

	$scope.send_add_request = function (tag_name_, parent_id_) {
		const params_object = {
			tag_name: tag_name_,
			parent_id: parent_id_
		};

		sender.send("add_tag.php", params_object).then(
		function (response) {
			logger.get_log().event(response.data.message);
			$scope.load_tags();
		}, function (response) {
			logger.get_log().error("Cannot add '" + tag_name + "' tag");
		});
	}

	$scope.send_delete_request = function (tag_id_, tag_name_) {
		const params_object = {
			tag_id: tag_id_,
		};

		sender.send("delete_tag.php", params_object).then(
		function (response) {
			logger.get_log().event(response.data.message);
			$scope.load_tags();
		}, function (response) {
			logger.get_log().error("Cannot delete '" + tag_name_ + "' tag");
		});
	}

	$scope.send_update_request = function (tag_id_, tag_name_) {
		const params_object = {
			tag_id: tag_id_,
			tag_name: tag_name_
		};

		sender.send("update_tag.php", params_object).then(
		function (response) {
			logger.get_log().event(response.data.message);
			$scope.load_tags();
		}, function (response) {
			logger.get_log().error("Cannot update '" + tag_name_ + "' tag");
		});
	}

	$scope.add_tag = function () {
		if ($scope.tree == null) {
			logger.get_log().error("Tree is not loaded");
			return;
		}

		let tag_name = read_lower_from_input("add_tag_name_input");
		let parent_name = read_lower_from_input("add_tag_parent_input");
		
		if (tag_name == "") {
			logger.get_log().error("Empty tag field");
			return;
		}

		if ($scope.tree.has_tag(tag_name)) {
			logger.get_log().error("Tag '" + tag_name + "' already exists");
			return;
		}
		
		if (parent_name != "" && !$scope.tree.has_tag(parent_name)) {
			logger.get_log().error("There is no such tag as '" + parent_name + "'");
			return;
		}
		
		let parent_id = $scope.tree.get_tag_id(parent_name);
		$scope.send_add_request(tag_name, parent_id);
	}

	$scope.delete_tag = function () {
		if ($scope.tree == null) {
			logger.get_log().error("Tree is not loaded");
			return;
		}

		let tag_name = read_lower_from_input("delete_tag_name_input");

		if (tag_name == "") {
			logger.get_log().error("Empty tag field");
			return;
		}

		if (!$scope.tree.has_tag(tag_name)) {
			logger.get_log().error("Tag '" + tag_name + "' does not exist");
			return;
		}

		let tag_id = $scope.tree.get_tag_id(tag_name);
		$scope.send_delete_request(tag_id, tag_name);
	}

	$scope.update_tag = function () {
		if ($scope.tree == null) {
			logger.get_log().error("Tree is not loaded");
			return;
		}

		const old_tag_name = read_lower_from_input("update_tag_input_old");
		const new_tag_name = read_lower_from_input("update_tag_input_new");

		if (old_tag_name == "" || new_tag_name == "") {
			logger.get_log().error("Empty tag field");
			return;
		}

		if (!$scope.tree.has_tag(old_tag_name)) {
			logger.get_log().error("Tag '" + old_tag_name + "' does not exist");
			return;
		}

		if ($scope.tree.has_tag(new_tag_name)) {
			logger.get_log().error("Tag '" + new_tag_name + "' already exists");
			return;
		}

		let old_tag_id = $scope.tree.get_tag_id(old_tag_name);
		$scope.send_update_request(old_tag_id, new_tag_name);
	}
	
});
