<?php

/*
Expects HTTP POST with fields:
	content {
	}

Creates RETURN_TAGS_TREE_REQUEST:
	[2]
*/

// in order to run on localhost
header("Access-Control-Allow-Origin: *");

header("Access-Control-Allow-Methods: POST");
header('Content-type: text/plain; charset=UTF-8');

require_once("tcp_ip.php");
require_once("utils.php");


$params = decode_input_message();
$message = prepare_message($params);
$response = send_message($message);
yeld_success($response);

function prepare_message($params){
	$str = "[2]";
	return $str;
}

?>