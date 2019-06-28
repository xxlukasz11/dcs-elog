function read_error_msg(response) {
	let error_msg = response.data.message;
	if (error_msg)
		return error_msg;
	else
		return "Connection failed: Server may be unavialiable.";
}

function read_from_input(input_id) {
	return document.getElementById(input_id).value.toLowerCase();
}
