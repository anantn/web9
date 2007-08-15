<?php

$content = file_get_contents('php://input');
$data = base64_decode($content);

$fd = fsockopen('tcp://127.0.0.1', 1564);
fwrite($fd, $data);

$out = "";
while ($out = fgets($fd)) {
	$out .= $out;
}
fclose($fd);

$toSend = base64_encode($out);

header('application/octet-stream');
echo $toSend;

?>
