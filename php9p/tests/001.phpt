--TEST--
Check for 9p presence
--SKIPIF--
<?php if (!extension_loaded("9p")) print "skip"; ?>
--FILE--
<?php 
echo "9p extension is available";
/*
	you can add regression tests for your extension here

  the output of your test code has to be equal to the
  text in the --EXPECT-- section below for the tests
  to pass, differences between the output and the
  expected text are interpreted as failure

	see php5/README.TESTING for further information on
  writing regression tests
*/
?>
--EXPECT--
9p extension is available
