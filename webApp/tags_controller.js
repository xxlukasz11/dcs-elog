
app.controller('edit_tags', function ($scope, sender, logger) {
	$scope.tree = null;

	$scope.load_tags = function ($event) {
		const button_handler = new Button_load_handler($event.target);
		button_handler.animate();
		$scope.send_tags_tree_request(button_handler);
	}

	$scope.trigger_load_tags = function () {
		const button_handler = new Button_load_handler();
		$scope.send_tags_tree_request(button_handler);
	}

	$scope.send_tags_tree_request = function (button_handler) {
		sender.send("load_tags.php", {}).then(
		function (response) {
			$scope.handle_tags_tree_response(response);
			button_handler.reset();
		}, function (response) {
			logger.get_log().error(read_error_msg(response));
			button_handler.reset();
		});
	}

	$scope.handle_tags_tree_response = function (response) {
		const response_data = response.data;
		$scope.tree = from_table_to_tree(response_data.data);
		let container = document.getElementById('tags_tree_container');
		$scope.tree.set_container(container);
		$scope.tree.display();
		logger.get_log().data(response_data);
	}

	$scope.send_add_request = function (tag_name_, parent_id_, button_handler) {
		const params_object = {
			tag_name: tag_name_,
			parent_id: parent_id_
		};

		sender.send("add_tag.php", params_object).then(
		function (response) {
			logger.get_log().event(response.data.message);
			$scope.trigger_load_tags();
			button_handler.reset();
		}, function (response) {
			logger.get_log().error("Cannot add '" + tag_name + "' tag");
			button_handler.reset();
		});
	}

	$scope.send_delete_request = function (tag_id_, tag_name_, button_handler) {
		const params_object = {
			tag_id: tag_id_,
		};

		sender.send("delete_tag.php", params_object).then(
		function (response) {
			logger.get_log().event(response.data.message);
			$scope.trigger_load_tags();
			button_handler.reset();
		}, function (response) {
			logger.get_log().error("Cannot delete '" + tag_name_ + "' tag");
			button_handler.reset();
		});
	}

	$scope.send_update_request = function (tag_id_, tag_name_, button_handler) {
		const params_object = {
			tag_id: tag_id_,
			tag_name: tag_name_
		};

		sender.send("update_tag.php", params_object).then(
		function (response) {
			logger.get_log().event(response.data.message);
			$scope.trigger_load_tags();
			button_handler.reset();
		}, function (response) {
			logger.get_log().error("Cannot update '" + tag_name_ + "' tag");
			button_handler.reset();
		});
	}

	$scope.add_tag = function ($event) {
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

		const button_handler = new Button_load_handler($event.target);
		button_handler.animate();
		$scope.send_add_request(tag_name, parent_id, button_handler);
	}

	$scope.delete_tag = function ($event) {
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
		const button_handler = new Button_load_handler($event.target);
		button_handler.animate();
		$scope.send_delete_request(tag_id, tag_name, button_handler);
	}

	$scope.update_tag = function ($event) {
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
		const button_handler = new Button_load_handler($event.target);
		button_handler.animate();
		$scope.send_update_request(old_tag_id, new_tag_name, button_handler);
	}
	
});
