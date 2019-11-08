<?php

function decode_input_message() {
	$post_message = json_decode(file_get_contents('php://input'), true);
	$params = json_decode($post_message["content"]);
	return $params;
}

?>