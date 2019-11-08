<?php

// in order to run on localhost
header("Access-Control-Allow-Origin: *");
header("Access-Control-Allow-Methods: POST");
header('Content-type: text/plain; charset=UTF-8');

require_once("tcp_ip.php");
require_once("utils.php");


$str = prepare_message();
	
$response = send_data($str);
	
// debug
//$response = '[{"tag":"a","parent":""},{"tag":"b","parent":"a"},{"tag":"c","parent":"a"},{"tag":"d","parent":"b"},{"tag":"e","parent":"b"},{"tag":"f","parent":"c"},{"tag":"g","parent":"c"},{"tag":"X","parent":null},{"tag":"Y","parent":"X"},{"tag":"Z","parent":"X"}]';
	
http_response_code(200);
echo $response;


function prepare_message(){
	$str = "[2]";
	return $str;
}

?>