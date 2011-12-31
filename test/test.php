<html>
<head>
<title>TEST</title>
<meta http-equiv="Content-Type" content="text/html; charset=iso-8859-1">
</head>
<body bgcolor="#b8c8fe" leftmargin="12" topmargin="12">
Test DF1 <br><br><br><br>
<?
$service_port = 17560;
$address = "localhost";
$socket = socket_create( AF_INET, SOCK_STREAM, 0 );
if( $socket < 0 ) {
    echo "socket_create() failed: reason: " . socket_strerror( $socket ) . "\n";
exit;
}

/* Attempt to connect to $address on $service_port */
if( !( socket_connect( $socket, $address, $service_port ) ) ) {
  $result = socket_last_error( $socket );
  socket_clear_error( $socket );
  print "Error connecting to $ip:$port -- " . socket_strerror( $result ) . " ($result)\n";
  exit;
}
function readSLC($tag) {
  global $socket;
  $value = "";
  socket_write( $socket, $tag, strlen( $tag ) );
  $value = socket_read( $socket, 128, PHP_NORMAL_READ );
  return trim($value);
}
function writeSLC($tag,$value) {
  global $socket;
  $tag = $tag ."=".$value;
  socket_write( $socket, $tag, strlen( $tag ) );
  $value = socket_read( $socket, 128, PHP_NORMAL_READ );
  return trim($value);
}
echo "Time SLC : ".readSLC('S2:40').":".readSLC('S2:41').":".readSLC('S2:42')."<BR>";
echo "<BR>";
echo "F8:0=".floatval(readSLC('F8:0'))."<BR>";
echo "<BR>";
echo "F8:1=".floatval(readSLC('F8:1'))."<BR>";
echo "<BR>";
echo "F8:2=".floatval(readSLC('F8:2'))."<BR>";
echo "<BR>";
echo "<BR>";

echo "N7:4= ".readSLC('n7:4')."<BR>";
//echo "write N7:4=10".writeSLC('n7:4','10')."<BR>";
echo "<BR>";

socket_close( $socket );

?>
</body>
</html>
