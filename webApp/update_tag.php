<?php

// in order to run on localhost
header("Access-Control-Allow-Origin: *");

header("Content-Type: application/json; charset=UTF-8");
header("Access-Control-Allow-Methods: GET");
header('Content-type: text/plain; charset=utf-8');

require_once("tcp_ip.php");

if(isset($_GET['content'])){
	$params = json_decode($_GET['content']);
	
	$str = prepare_message($params);
	$response = send_data($str);
	
	http_response_code(200);
	echo $response;
}

function prepare_message($params){
	$tag_id = $params->tag_id;
	$tag_id_len = strlen($tag_id);

	$tag_name = $params->tag_name;
	$tag_name_len = strlen($tag_name);
	
	$str = "[5][$tag_id_len $tag_name_len][$tag_id][$tag_name]";
	return $str;
}

?>