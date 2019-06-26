class Event {
	constructor(type, message) {
		this.type = type;
		this.message = message;
	}
}

class Error_event extends Event {
	constructor(message) {
		super("error", message);
	}
}

class Success_event extends Event {
	constructor(message) {
		super("success", message);
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
}