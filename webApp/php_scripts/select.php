<?php

/*
Expects HTTP POST with fields:
	content {
		min_date,
		max_date,
		tags
	}

Creates RETURN_EVENTS_REQUEST:
	[1]
	[...]
	[min_date]
	[max_date]
	[tags]
	[limit] (not supported yet)
	[offset] (not supported yet)
*/

// in order to run on localhost
header("Access-Control-Allow-Origin: *");

header("Access-Control-Allow-Methods: POST");
header('Content-type: text/plain; charset=UTF-8');

require_once("tcp_ip.php");
require_once("utils.php");


// debug
//$params = json_decode('{"min_date":"","max_date":"","tags":""}');
	
$params = decode_input_message();
$message = prepare_message($params);
send_return_event_request($message);
http_response_code(200);

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
	
	$str = "[1][$min_date_len $max_date_len $tags_len 0 0][$min_date][$max_date][$tags][][]";
	return $str;
}

?>