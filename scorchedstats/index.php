<?
include('config.php');

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

<p align=center><b>Stats for Version 37 (Started on 28-04-2004)</b></p>

<?
// Top Players Query
$query = " select wins, overallwinner, osdesc, playerid, kills, name from scorched3d_players order by kills desc limit 10";
$result = mysql_query($query) or die("Query failed : " . mysql_error());
?>
<table width=500 border="0" align="center">
<tr><td align=center><b>Top 10 Killers For All Time</b></td></tr>
</table>
<table width=500 bordercolor=#333333 cellspacing="0" cellpadding="0" border="1" align="center">
<tr>
<td bgcolor=#111111 width=50></td>
<td bgcolor=#111111><b>Player Name</b></td>
<td bgcolor=#111111><b>Kills</b></td>
<td bgcolor=#111111><b>Round Wins</b></td>
<td bgcolor=#111111><b>Game Wins</b></td>
<td bgcolor=#111111><b>OS</b></td>
</tr>
<?
$rownum=0;	
while ($row = mysql_fetch_object($result))
{
	$tmp = preg_replace("/[\(-].*/", "", $row->osdesc);

	++$rownum;
	echo "<tr>";
	echo "<td>$rownum</td>";
	echo "<td><a href=\"playerstats.php?PlayerID=$row->playerid\">$row->name</a></td>";
	echo "<td>$row->kills</td>";
	echo "<td>$row->wins</td>";
	echo "<td>$row->overallwinner</td>";
	echo "<td>$tmp</td>";
	echo "</tr>";
}
?>
</table>
<?
$query = "select count(playerid) from scorched3d_players;";
$result = mysql_query($query) or die("Query failed : " . mysql_error());
$row = mysql_fetch_array($result);
?>
<table width=500 border =0 align=center>
<tr><td><a href="allplayers.php?PlayerID=0">
View all players (<? print $row[0]; ?>). 
</a>
</tr></td>
</table>
<br>

<?
// Top Yesterday Players Query
$query = "select (scorched3d_events.playerid) as playerid, (scorched3d_players.name) as name, (scorched3d_players.osdesc) as osdesc, (count(*)) as kills from scorched3d_events LEFT JOIN scorched3d_players ON (scorched3d_events.playerid=scorched3d_players.playerid)  where scorched3d_events.eventtype = 1 and TO_DAYS(NOW()) - TO_DAYS(scorched3d_events.eventtime) = 1 group by playerid order by kills desc limit 10";
$result = mysql_query($query) or die("Query failed : " . mysql_error());
?>
<table width=500 border="0" align="center">
<tr><td align=center><b>Top 10 Killers From Yesterday</b></td></tr>
</table>
<table width=500 bordercolor=#333333 cellspacing="0" cellpadding="0" border="1" align="center">
<tr>
<td bgcolor=#111111 width=50></td>
<td bgcolor=#111111><b>Player Name</b></td>
<td bgcolor=#111111><b>Kills</b></td>
<td bgcolor=#111111><b>OS</b></td>
</tr>
<?
$rownum=0;
while ($row = mysql_fetch_object($result))
{
	$tmp = preg_replace("/[\(-].*/", "", $row->osdesc);

        ++$rownum;
        echo "<tr>";
        echo "<td>$rownum</td>";
        echo "<td><a href=\"playerstats.php?PlayerID=$row->playerid\">$row->name</a></td>";
        echo "<td>$row->kills</td>";
	echo "<td>$tmp</td>";
        echo "</tr>";
}
?>
</table>
<?
$query = "select count(playerid) from scorched3d_players;";
$result = mysql_query($query) or die("Query failed : " . mysql_error());
$row = mysql_fetch_array($result);
?>
<table width=500 border =0 align=center>
<tr><td><a href="allplayers.php?PlayerID=0">
View all players (<? print $row[0]; ?>). 
</a>
</tr></td>
</table>
<br>

<?
// Top Last Week Query
$query = "select (scorched3d_events.playerid) as playerid, (scorched3d_players.name) as name, (scorched3d_players.osdesc) as osdesc, (count(*)) as kills from scorched3d_events LEFT JOIN scorched3d_players ON (scorched3d_events.playerid=scorched3d_players.playerid)  where scorched3d_events.eventtype = 1 and TO_DAYS(NOW()) - TO_DAYS(scorched3d_events.eventtime) >= 1 and TO_DAYS(NOW()) - TO_DAYS(scorched3d_events.eventtime) <= 7 group by playerid order by kills desc limit 10";
$result = mysql_query($query) or die("Query failed : " . mysql_error());
?>
<table width=500 border="0" align="center">
<tr><td align=center><b>Top 10 Killers From Last 7 Days</b></td></tr>
</table>
<table width=500 bordercolor=#333333 cellspacing="0" cellpadding="0" border="1" align="center">
<tr>
<td bgcolor=#111111 width=50></td>
<td bgcolor=#111111><b>Player Name</b></td>
<td bgcolor=#111111><b>Kills</b></td>
<td bgcolor=#111111><b>OS</b></td>
</tr>
<?
$rownum=0;
while ($row = mysql_fetch_object($result))
{
        $tmp = preg_replace("/[\(-].*/", "", $row->osdesc);

        ++$rownum;
        echo "<tr>";
        echo "<td>$rownum</td>";
        echo "<td><a href=\"playerstats.php?PlayerID=$row->playerid\">$row->name</a></td>";
        echo "<td>$row->kills</td>";
        echo "<td>$tmp</td>";
        echo "</tr>";
}
?>
</table>
<?
$query = "select count(playerid) from scorched3d_players;";
$result = mysql_query($query) or die("Query failed : " . mysql_error());
$row = mysql_fetch_array($result);
?>
<table width=500 border =0 align=center>
<tr><td><a href="allplayers.php?PlayerID=0">
View all players (<? print $row[0]; ?>).
</a>
</tr></td>
</table>
<br>


<?
// Top weapons query
$query=" select weaponid, name, kills from scorched3d_weapons where kills > 0 group by weaponid order by kills desc";
$result = mysql_query($query) or die("Query failed : " . mysql_error());
?>
<table width="500" border="0" align="center">
<tr><td align=center><b>Top Weapon Kills For All Time</b></td></tr>
</table>
<table width="500" bordercolor=#333333 cellspacing="0" cellpadding="0" border="1" align="Center">
<tr>
<td bgcolor=#111111 witdh=50></td>
<td bgcolor=#111111><b>Weapon</b></td>
<td bgcolor=#111111><b>Kills</b></td>
</tr>
<?
$rownum=0;	
while ($row = mysql_fetch_object($result))
{
	++$rownum;	
	echo "<tr>";
	echo "<td>$rownum</td>";
	echo "<td><a href=\"weaponstats.php?WeaponID=$row->weaponid\">$row->name</a></td>";
	echo "<td>$row->kills</td>";
	echo "</tr>";
}
?>
</table><br>

<?
// Servers
$query=" select * from scorched3d_main";
$result = mysql_query($query) or die("Query failed : " . mysql_error());
?>
<table width="500" border="0" align="center">
<tr><td align=center><b>Participating Servers</b></td></tr>
</table>
<table width="500" bordercolor=#333333 cellspacing="0" cellpadding="0" border="1" align="Center">
<tr>
<td bgcolor=#111111><b>Server</b></td>
<td bgcolor=#111111><b>Total Rounds</b></td>
<td bgcolor=#111111><b>Total Games</b></td>
</tr>
<?
$rownum=0;
while ($row = mysql_fetch_object($result))
{
        ++$rownum;
        echo "<tr>";
        echo "<td>$row->name</td>";
        echo "<td>$row->rounds</td>";
        echo "<td>$row->games</td>";
        echo "</tr>";
}
?>
</table><br>

<?
include('statsfooter.php');
?>
</body>
</html>
