<?php

// in order to run on localhost
header("Access-Control-Allow-Origin: *");
header("Access-Control-Allow-Methods: POST");
header('Content-type: text/plain; charset=UTF-8');

require_once("tcp_ip.php");
require_once("utils.php");


$params = decode_input_message();
	
$str = prepare_message($params);
$response = send_data($str);
	
http_response_code(200);
echo $response;


function prepare_message($params){
	$tag_id = $params->tag_id;
	$tag_id_len = strlen($tag_id);
	
	$str = "[4][$tag_id_len][$tag_id]";
	return $str;
}

?>