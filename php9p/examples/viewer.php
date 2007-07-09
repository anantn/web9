<html>
<head>
	<title>9P File Viewer</title>
</head>
<body>

<?php
if (!isset($_REQUEST['address'])) {	
?>
	
	<form name="ixp" method="get" action="viewer.php">
	Address to Dial:
		<input type="text" name="address" />
		<input type="submit" value="View!" />
	</form>
	
	<p>
	Example: tcp!sources.cs.bell-labs.com!564
	</p>

<?php
} else {

	$address = $_REQUEST['address'];
	if (!isset($_REQUEST['path'])) {
		$path = '/';
		$type = 'd';
	} else {
		$path = $_REQUEST['path'];
		$type = $_REQUEST['type'];
	}

	$client = new IxpClient($address);

	if ($type == 'd') {

		$stats = $client->statDir($path);
		$names = array();
	
		foreach ($stats as $stat) {
			$mode = $stat->getMode();
			$time = $stat->getTime();
			$names[$stat->name] = array($mode, $stat->uid, $stat->gid, $time);
		}

		ksort($names);

		echo '<table width="80%">';
		echo '<td></td>';
		echo '<td><b>Name</b></td>';
		echo '<td><b>Mode</b></td>';
		echo '<td><b>User</b></td>';
		echo '<td><b>Group</b></td>';
		echo '<td><b>Last Modified</b></td>';

		foreach ($names as $name=>$details) {
			$toType = $details[0][0];
			echo '<tr>';
			if ($toType == 'd')
				echo '<td><img src="folder.gif" /></td>';
			else
				echo '<td><img src="file.gif" /></td>';

			$add = urlencode($address);
			$nam = urlencode($path."/".$name);
			echo "<td><a href=\"viewer.php?address=$add&path=$nam&type=$toType\">$name</td>";
			foreach ($details as $detail)
				echo '<td>'.$detail.'</td>';
		
			echo '</tr>';
		}

		echo '</table>';
		$client->unmount();
		echo "<p>Browsing $address</p>";

	} else {

		$cfid = $client->open($path);
		while ($buf = $cfid->read($cfid->iounit))
			echo $buf;
		$cfid->close();
		$client->unmount();

	}	
}
?>

<p>Powered by <a href="http://code.kix.in/projects/web9">Web9</a></p>
</body>
</html>
