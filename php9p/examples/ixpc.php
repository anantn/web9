<?php

function usage()
{
	echo "usage: php ixpc.php <address> {read | ls} <file>\n";
	exit;
}

function ls($client, $addr, $file)
{
	$stats = $client->statDir($file);
	$names = array();

	foreach ($stats as $stat) {
		$mode = $stat->getMode();
		$time = $stat->getTime();
		$names[$stat->name] = $mode."\t".$stat->uid."\t".$stat->gid."\t".$time;
	}
	ksort($names);
	foreach ($names as $name=>$det)
		echo $det."\t".$name."\n";
}

function read($client, $addr, $file)
{
	$fid = $client->open($file);
	while ($buf = $fid->read($fid->iounit))
		echo $buf;
}

if (($argc != 4)) {
	usage();
}

$client = new IxpClient($argv[1]);

switch($argv[2]) {
	case "read"	: read($client, $argv[1], $argv[3]); break;
	case "ls"	: ls($client, $argv[1], $argv[3]); break;
	default		: usage();
}

$client->unmount();

?>
