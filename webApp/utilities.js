function read_error_msg(response) {
	let error_msg = response.data.message;
	if (error_msg)
		return error_msg;
	else
		return "Connection failed: PHP server may be unavialiable.";
}

function convert_description_to_send(description) {
	return description.replace(/\r?\n/g, '<br/>');
}

function read_from_input(input_id) {
	return document.getElementById(input_id).value;
}

function read_lower_from_input(input_id) {
	return document.getElementById(input_id).value.toLowerCase();
}