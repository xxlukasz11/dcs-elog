class Update_event_manager {
	constructor(control_button) {
		this.edit_view = [];
		this.display_view = [];
		this.title_label = null;
		this.title_input = null;
		this.tags_label = null;
		this.tags_input = null;
		this.description_label = null;
		this.description_input = null;

		this.init(control_button);
	}

	init(control_button) {
		const table_body = this.get_parent_by_class_name(control_button, 'table_body');

		const event_row = table_body.getElementsByClassName('event_info')[0];
		this.add_elements_to_view(event_row);
		this.set_heading_elements(event_row);

		const description_row = table_body.getElementsByClassName('description_row')[0];
		this.add_elements_to_view(description_row);
		this.set_descrption_element(description_row);
	}

	set_heading_elements(event_row) {
		const title_wrapper = event_row.getElementsByClassName('event_title')[0];
		this.title_label = title_wrapper.getElementsByTagName('span')[0];
		this.title_input = title_wrapper.getElementsByTagName('input')[0];

		const tags_wrapper = event_row.getElementsByClassName('event_tags')[0];
		this.tags_label = tags_wrapper.getElementsByTagName('span')[0];
		this.tags_input = tags_wrapper.getElementsByTagName('input')[0];
	}

	set_descrption_element(description_row) {
		this.description_label = description_row.getElementsByClassName('description_field')[0];
		this.description_input = description_row.getElementsByClassName('description_input')[0];
	}

	add_elements_to_view(parent_element) {
		const edit_elements = parent_element.getElementsByClassName('edit_view')
		for(let element of edit_elements)
			this.edit_view.push(element);

		const display_elements = parent_element.getElementsByClassName('display_view');
		for(let element of display_elements)
			this.display_view.push(element);
	}

	hide_element(element) {
		element.classList.add('hidden');
	}

	show_element(element) {
		element.classList.remove('hidden');
	}

	get_parent_by_class_name(element, class_name) {
		let tmp = element;
		while (tmp.parentElement != null) {
			const parent = tmp.parentElement;
			if (parent.classList.contains(class_name))
				return parent;
			tmp = parent;
		}
		return null;
	}

	edit_mode() {
		for(let element of this.display_view)
			this.hide_element(element);
		for(let element of this.edit_view)
			this.show_element(element);
	}

	display_mode() {
		for(let element of this.edit_view)
			this.hide_element(element);
		for(let element of this.display_view)
			this.show_element(element);
	}

	get_title_from_input() {
		return this.title_input.value;
	}

	get_tags_from_input() {
		return this.tags_input.value;
	}

	get_description_from_input() {
		return this.description_input.value;
	}
}