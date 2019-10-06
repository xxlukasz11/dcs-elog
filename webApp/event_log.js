class Abstract_event {
	constructor(type, message) {
		this.type = type;
		this.message = message;
		this.time = new Date().toISOString().slice(11, 19);
	}
}

class Error_event extends Abstract_event {
	constructor(message) {
		super("error", message);
	}
}

class Success_event extends Abstract_event {
	constructor(message) {
		super("success", message);
	}
}

class Event extends Abstract_event {
	constructor(message) {
		super("event", message);
	}
}

class Event_log {
	constructor() {
		this.events = [];
	}

	error(message) {
		this.events.unshift(new Error_event(message));
	}

	success(message) {
		this.events.unshift(new Success_event(message));
	}

	event(message) {
		this.events.unshift(new Event(message));
	}

	data(response_data) {
		if (response_data.error_code == 0) {
			this.success(response_data.message);
		}
		else {
			this.event(response_data.message);
		}
	}
}

class Console_logger {

	log_message(message_name, parameter_object) {
		console.log("=== " + message_name + " ===");
		this.log_object(parameter_object);
	}

	log_object(object) {
		console.log(object);
	}
}