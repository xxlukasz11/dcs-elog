<?php

function create_json_response($message){
	$obj = array('message' => $message);
	$msg = json_encode($obj);
	return $msg;
}

function send_data($data){
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
		$msg = create_json_response('Internal server error: CREATE_ERROR');
	    http_response_code(500);
		echo $msg;
		exit();
	}
	
	if (!socket_set_option($socket, SOL_SOCKET, SO_REUSEADDR, 1)) {
		$msg = create_json_response('Internal server error: REUSEADDR_ERROR');
	    http_response_code(500);
		echo $msg;
		exit();
	}
	
	if (!socket_set_option($socket, SOL_SOCKET, SO_RCVTIMEO, array("sec" => 10, "usec" => 0))) {
		$msg = create_json_response('Internal server error: RCVTIMEO_ERROR');
	    http_response_code(500);
		echo $msg;
		exit();
	}

	$conn = socket_connect($socket, $server_address, $server_port);
	if ($conn === false) {
		$msg = create_json_response('Cannot connect to the server');
	    http_response_code(500);
		echo $msg;
		exit();
	}

	$resp = '';
	
	$length = strlen($data);
	$binary_length = strrev( pack("N", $length) );
	
	socket_send($socket, $binary_length, 4, 0);
	socket_send($socket, $data, $length, 0);
	
	while ($bff = socket_read($socket, 1024)) {
		$resp .= $bff;
	}
	
	socket_close($socket);
	return $resp;
}

?>