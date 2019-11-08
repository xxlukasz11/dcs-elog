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
	$tag_name = $params->tag_name;
	$tag_name_len = strlen($tag_name);
	
	$parent_id = $params->parent_id;
	$parent_id_len = strlen($parent_id);
	
	$str = "[3][$tag_name_len $parent_id_len][$tag_name][$parent_id]";
	return $str;
}

?>