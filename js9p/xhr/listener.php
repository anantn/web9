<?php

$content = file_get_contents('php://input');
$messages = explode(',', $content);
$sock = fsockopen('tcp://127.0.0.1', 1564);
$responses = array();

foreach($messages as $message) {
	fwrite($sock, base64_decode($message));
	$res  = fread($sock, 4);
	$size = unpack("V", $res);
	$size = $size[1];

	$msg = fread($sock, $size - 4);
	$responses[] = base64_encode($res.$msg);
}

echo json_encode($responses);
fclose($sock);


?>
