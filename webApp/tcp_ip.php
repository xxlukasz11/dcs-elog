<?php

function send_data($data){
	error_reporting(E_ALL);
	
	$server_port = 9100;
	
	// remote server
	//$server_address = "194.29.174.79";
	
	// localhost
	$server_address = "127.0.0.1";

	$socket = socket_create(AF_INET, SOCK_STREAM, SOL_TCP);
	if ($socket === false) {
	    http_response_code(-1);
		echo "socket_create() failed: reason: " . socket_strerror(socket_last_error()) . PHP_EOL;
		exit(1);
	}
	
	if (!socket_set_option($socket, SOL_SOCKET, SO_REUSEADDR, 1)) {
	    http_response_code(-1);
		echo 'Unable to set option on socket: '. socket_strerror(socket_last_error()) . PHP_EOL;
		exit(1);
	}
	
	if (!socket_set_option($socket, SOL_SOCKET, SO_RCVTIMEO, array("sec" => 10, "usec" => 0))) {
	    http_response_code(-1);
		echo 'Unable to set option on socket: '. socket_strerror(socket_last_error()) . PHP_EOL;
		exit(1);
	}

	$result = socket_connect($socket, $server_address, $server_port);
	if ($result === false) {
	    http_response_code(-1);
		echo "socket_connect() failed.\nReason: ($result) " . socket_strerror(socket_last_error($socket)) . PHP_EOL;
		exit(1);
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