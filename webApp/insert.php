<?php

// in order to run on localhost
header("Access-Control-Allow-Origin: *");
header("Access-Control-Allow-Methods: POST");
header('Content-type: text/plain; charset=UTF-8');

require_once("tcp_ip.php");
require_once("utils.php");


$params = decode_input_message();
	
// debug
//$params = json_decode('{"title":"title","description":"d","tags":"a,b,c"}');
	
$str = prepare_message($params);
$response = send_data($str);
	
http_response_code(200);
echo $response;


function create_attachment_info_array($attachments) {
	$info_array = array();
	foreach($attachments as $att) {
		array_push($info_array, "$att->name;$att->type");
	}
	return $info_array;
}

function prepare_message($params){
	$title = $params->title;
	$title_len = strlen($title);
	
	$description = $params->description;
	$description_len = strlen($description);
	
	$tags = $params->tags;
	$tags_len = strlen($tags);

	$author = $params->author;
	$author_len = strlen($author);

	$attachment_info = create_attachment_info_array($params->attachments);
	$attachment_info_str = implode($attachment_info, ' ');
	$attachment_info_str_len = strlen($attachment_info_str);
	
	$str = "[0][$title_len $description_len $tags_len $author_len $attachment_info_str_len]
				[$title][$description][$tags][$author][$attachment_info_str]";
	return $str;
}

?>