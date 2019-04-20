<?php

// in order to run on localhost
header("Access-Control-Allow-Origin: *");

header("Content-Type: application/json; charset=UTF-8");
header("Access-Control-Allow-Methods: GET");

require_once("tcp_ip.php");

if(isset($_GET['content'])){
	$params = json_decode($_GET['content']);
	
	// debug
	//$params = json_decode('{"min_date":"","max_date":"","tags":""}');
	
	$str = prepare_message($params);
	$response = send_data($str);
	
	http_response_code(200);
	echo $response;
}

function prepare_message($params){
	$min_date = $params->min_date;
	$min_date_len = strlen($min_date);
	if($min_date_len != 10){
		$min_date = "";
		$min_date_len = 0;
	}
	else{
		$min_date .= " 00:00:00";
		$min_date_len = strlen($min_date);
	}
	
	$max_date = $params->max_date;
	$max_date_len = strlen($max_date);
	if($max_date_len != 10){
		$max_date = "";
		$max_date_len = 0;
	}
	else{
		$max_date .= " 23:59:59";
		$max_date_len = strlen($max_date);
	}
	
	$tags = $params->tags;
	$tags_len = strlen($tags);
	
	$str = "[1][$min_date_len $max_date_len $tags_len][$min_date][$max_date][$tags]";
	return $str;
}

?>