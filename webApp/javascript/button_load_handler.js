class Button_load_handler {

	constructor(button) {
		if (button) {
			this.button = button;
		}
		else {
			this.button = document.createElement('button');
		}
		
		this.loading_icon = this.create_loading_icon();
		this.loading = false;
	}

	create_loading_icon() {
		const icon = document.createElement('i');
		icon.className = "fa fa-refresh fa-spin";
		icon.style.marginRight = "5px";
		return icon;
	}

	animate() {
		if (!this.loading) {
			this.loading = true;
			this.button.disabled = true;
			this.appendIcon();
		}
	}

	appendIcon() {
		const firstChild = this.button.firstChild;
		if (firstChild) {
			this.button.insertBefore(this.loading_icon, firstChild);
		}
		else {
			this.button.appendChild(this.loading_icon);
		}
	}

	reset() {
		if (this.loading) {
			this.button.disabled = false;
			this.button.removeChild(this.loading_icon);
			this.loading = false;
		}
	}

}
