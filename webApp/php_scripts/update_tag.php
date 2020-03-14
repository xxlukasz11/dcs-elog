<?php

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
	$tag_id = $params->tag_id;
	$tag_id_len = strlen($tag_id);

	$tag_name = $params->tag_name;
	$tag_name_len = strlen($tag_name);
	
	$str = "[5][$tag_id_len $tag_name_len][$tag_id][$tag_name]";
	return $str;
}

?>