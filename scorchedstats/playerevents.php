<?
include('config.php');

$playerid=$_GET['PlayerID'] or die ("No player specified");
$link = mysql_connect($dbhost, $dbuser, $dbpasswd) or die("Could not connect : " . mysql_error());
mysql_select_db($dbname) or die("Could not select database");
?>

<!DOCTYPE HTML PUBLIC "-//W3C//DTD HTML 4.01 Transitional//EN">
<html>
<head><title>Scorched3D Player Stats</title></head>
<body bgcolor="#000000" text="#FFFFFF" link="#FF9933" vlink="#FF9933" alink="#FF9933">

<?
include('statsheader.php');
?>

<?
// General Player Stats
$query = "SELECT * FROM scorched3d_events where playerid=$playerid order by eventtime";
$result = mysql_query($query) or die("Query failed : " . mysql_error());
$row = mysql_fetch_object($result);
?>
<table width="500" border="0" align="center">
<tr><td align=center><b>Player Events</b></td></tr>
</table>
<table width="500" bordercolor=#333333 cellspacing="0" cellpadding="0" border="1" align="center">
<tr>
<td bgcolor=#111111><b>Time</b></td>
<td bgcolor=#111111><b>Event</b></td>
</tr>
<?
while ($row = mysql_fetch_object($result))
{
	$event = "none";
	switch ($row->eventtype)
	{
	case 1: $event = "killed"; break;
	case 2: $event = "team killed"; break;
	case 3: $event = "killed self"; break;
	case 4: $event = "resigned"; break;
	case 5: $event = "won"; break;
	case 6: $event = "overall winner"; break;
	}

        echo "<tr>";
        echo "<td>$row->eventtime</td>";
        echo "<td>$event</td>";
        echo "</tr>";
}
?>
</table>

<?
include('statsfooter.php');
?>
</body>
</html>

