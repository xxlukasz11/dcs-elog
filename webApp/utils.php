<?php

function decode_input_message() {
	$post_message = json_decode(file_get_contents('php://input'), true);
	$params = json_decode($post_message["content"]);
	return $params;
}

function send_attachments($socket, $params) {
	$attachments = $params->attachments;
	foreach($attachments as $att) {
		send_payload($socket, $att->payload);
	}
}

function create_attachment_info_array($attachments) {
	$info_array = array();
	foreach($attachments as $att) {
		array_push($info_array, "$att->name;$att->type");
	}
	return $info_array;
}

function send_message($message) {
	$socket = create_server_connection();
	send_payload($socket, $message);
	$response = read_response($socket);
	close_server_connection($socket);
	return $response;
}

function send_return_event_request($message) {
	$socket = create_server_connection();
	send_payload($socket, $message);
	read_response_and_forward($socket);
	close_server_connection($socket);
}

function send_message_with_attachments($message, $params) {
	$socket = create_server_connection();
	send_payload($socket, $message);
	send_attachments($socket, $params);
	$response = read_response($socket);
	return $response;
}

function yeld_success($response) {
	http_response_code(200);
	echo $response;
}

?>