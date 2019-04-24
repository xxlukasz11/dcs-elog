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
	//$params = json_decode('{"title":"title","description":"d","tags":"a,b,c"}');
	
	$str = prepare_message($params);
	$response = send_data($str);
	
	http_response_code(200);
	//echo $response;
}

function prepare_message($params){
	$title = $params->title;
	$title_len = strlen($title);
	
	$description = $params->description;
	$description_len = strlen($description);
	
	$tags = $params->tags;
	$tags_len = strlen($tags);
	
	$str = "[0][$title_len $description_len $tags_len][$title][$description][$tags]";
	return $str;
}

?>