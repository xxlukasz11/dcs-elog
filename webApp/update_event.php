<?php

// in order to run on localhost
header("Access-Control-Allow-Origin: *");

header("Content-Type: application/json; charset=UTF-8");
header("Access-Control-Allow-Methods: GET");
header('Content-type: text/plain; charset=utf-8');

require_once("tcp_ip.php");

if(isset($_GET['content'])){
	$params = json_decode($_GET['content']);
	
	// debug
	//$params = json_decode('{"id":57,"title":"title","description":"d","tags":"a,b,c"}');
	
	$str = prepare_message($params);
	$response = send_data($str);
	
	http_response_code(200);
	echo create_json_response($response);
}

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