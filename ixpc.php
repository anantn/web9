<?php

function usage()
{
	echo "usage: php ixpc.php <address> {read | write | remove} <file>\n";
	exit;
}

function ls($client, $addr, $file)
{
	$stat = $client->stat($file);
	var_dump($stat);
	var_dump(IXP_DMDIR);
}

function read($client, $addr, $file)
{
	$fid = $client->open($file);
	while ($buf = $fid->read($fid->iounit))
		echo $buf;
}

if ($argc != 4) {
	usage();
}

$client = new IxpClient($argv[1]);

switch($argv[2]) {
	case "read"	: read($client, $argv[1], $argv[3]); break;
	case "ls"	: ls($client, $argv[1], $argv[3]); break;
	default		: usage();
}

$client->unmount();
