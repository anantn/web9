<?php

$content = file_get_contents('php://input');
$data = base64_decode($content);

//$fd = fsockopen('tcp://127.0.0.1', 1564);
//$x = fwrite($fd, $data);

//$size = fread($fd, 4);
//fclose($fd);

//header('application/octet-stream');
echo base64_encode($data);

?>
