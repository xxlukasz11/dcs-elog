<?php

// in order to run on localhost
header("Access-Control-Allow-Origin: *");

header("Content-Type: application/json; charset=UTF-8");
header("Access-Control-Allow-Methods: GET");
header('Content-type: text/plain; charset=utf-8');

require_once("tcp_ip.php");

if(isset($_GET['content'])){
	$str = prepare_message();
	
	$response = send_data($str);
	
	// debug
	//$response = '[{"tag":"a","parent":""},{"tag":"b","parent":"a"},{"tag":"c","parent":"a"},{"tag":"d","parent":"b"},{"tag":"e","parent":"b"},{"tag":"f","parent":"c"},{"tag":"g","parent":"c"},{"tag":"X","parent":null},{"tag":"Y","parent":"X"},{"tag":"Z","parent":"X"}]';
	
	http_response_code(200);
	echo $response;
}

function prepare_message(){
	$str = "[2]";
	return $str;
}

?>