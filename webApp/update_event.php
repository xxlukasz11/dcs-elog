<?php

// in order to run on localhost
header("Access-Control-Allow-Origin: *");
header("Access-Control-Allow-Methods: POST");
header('Content-type: text/plain; charset=UTF-8');

require_once("tcp_ip.php");
require_once("utils.php");


$params = decode_input_message();
	
// debug
//$params = json_decode('{"id":57,"title":"title","description":"d","tags":"a,b,c"}');
	
$str = prepare_message($params);
$response = send_data($str);
	
http_response_code(200);
echo $response;


function prepare_message($params){
	$id = $params->id;
	$id_len = strlen($id);

	$title = $params->title;
	$title_len = strlen($title);
	
	$description = $params->description;
	$description_len = strlen($description);
	
	$tags = $params->tags;
	$tags_len = strlen($tags);
	
	$str = "[6][$id_len $title_len $description_len $tags_len][$id][$title][$description][$tags]";
	return $str;
}

?>