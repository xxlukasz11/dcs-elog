<?php

function create_json_response($message){
	$obj = array('message' => $message);
	$msg = json_encode($obj);
	return $msg;
}

function yeld_error($error_code, $message) {
	$msg = create_json_response($message);
	http_response_code($error_code);
	echo $msg;
}

function create_server_connection() {
	// debug
	//error_reporting(E_ALL);
	
	//release
	error_reporting(E_ERROR);
	
	$server_port = 9100;
	
	// remote server
	//$server_address = "194.29.174.79";
	
	// localhost
	$server_address = "127.0.0.1";

	$socket = socket_create(AF_INET, SOCK_STREAM, SOL_TCP);
	if ($socket === false) {
		
		exit();
	}
	
	if (!socket_set_option($socket, SOL_SOCKET, SO_REUSEADDR, 1)) {
		yeld_error(500, 'Internal server error: REUSEADDR_ERROR');
		exit();
	}
	
	if (!socket_set_option($socket, SOL_SOCKET, SO_RCVTIMEO, array("sec" => 10, "usec" => 0))) {
		yeld_error(500, 'Internal server error: RCVTIMEO_ERROR');
		exit();
	}

	$conn = socket_connect($socket, $server_address, $server_port);
	if ($conn === false) {
		yeld_error(500, 'Cannot connect to the remote server');
		exit();
	}

	return $socket;
}

function read_response($socket) {
	$resp = '';
	while ($bff = socket_read($socket, 1024)) {
		$resp .= $bff;
	}
	
	socket_close($socket);
	return $resp;
}

function send_data($socket, $data){
	$length = strlen($data);
	send_payload($socket, $length, $data);
}

function send_payload($socket, $size, $payload) {
	$binary_size = strrev( pack("N", $size) );
	socket_send($socket, $binary_size, 4, 0);

	$bytes_sent = 0;
	while ($bytes_sent < $size) {
		$remaining = $size - $bytes_sent;
		$bytes_sent += socket_send($socket, $payload[$bytes_sent], $remaining, 0);
	}
}

?>