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
$query = "SELECT * FROM scorched3d_players where playerid=$playerid";
$result = mysql_query($query) or die("Query failed : " . mysql_error());
$row = mysql_fetch_object($result);
?>
<table width="500" border="0" align="center">
<tr><td align=center><b>Player Stats</b></td></tr>
</table>
<table width="500" bordercolor=#333333 cellspacing="0" cellpadding="0" border="1" align="center">
<tr><td bgcolor=#111111><b>Last Used Player Name</b></td><td><?=$row->name?></td></tr>
<tr><td bgcolor=#111111><b>Last Connected</b></td><td><?=$row->lastconnected?></td></tr>
<tr><td bgcolor=#111111><b>OS</b></td><td><?=$row->osdesc?></td></tr>
<tr><td bgcolor=#111111><b>Player ID</b></td><td><?=$row->playerid?></td></tr>
<tr><td bgcolor=#111111><b>Times Connected</b></td><td><?=$row->connects?></td></tr>
<tr><td bgcolor=#111111><b>Games Won</b></td><td><?=$row->overallwinner?></td></tr>
<tr><td bgcolor=#111111><b>Rounds Won</b></td><td><?=$row->wins?></td></tr>
<tr><td bgcolor=#111111><b>Rounds Resigned</b></td><td><?=$row->resigns?></td></tr>
<tr><td bgcolor=#111111><b>Total Shots</b></td><td><?=$row->shots?></td></tr>
<tr><td bgcolor=#111111><b>Opponent Kills</b></td><td><?=$row->kills?></td></tr>
<tr><td bgcolor=#111111><b>Self Kills</b></td><td><?=$row->selfkills?></td></tr>
<tr><td bgcolor=#111111><b>Team Kills</b></td><td><?=$row->teamkills?></td></tr>
<tr><td bgcolor=#111111><b>Deaths</b></td><td><?=$row->deaths?></td></tr>
</table>
<table width="500" border="0" align="center">
<tr><td><a href="playerevents.php?PlayerID=<?=$playerid?>">
View all this players events.</a></td></tr>
</table>
<br>

<?
// General Player Stats
$query = "SELECT * FROM scorched3d_names where playerid=$playerid";
$result = mysql_query($query) or die("Query failed : " . mysql_error());
?>
<table width="500" border="0" align="center">
<tr><td align=center><b>Player Aliases</b></td></tr>
</table>
<table width="500" bordercolor=#333333 cellspacing="0" cellpadding="0" border="1" align="center">
<tr>
<td bgcolor=#111111><b>Player Alias</b></td>
<td bgcolor=#111111><b>Times Used</b></td>
</tr>
<?
	while ($row = mysql_fetch_object($result))
	{
		echo "<tr><td>$row->name</td><td>$row->count</td></tr>\n";
	}
?>
</table>
<br>
	
<?
// List top targets
$query="select (scorched3d_events.otherplayerid) as killedid, (scorched3d_players.name) as name, (count(*)) AS tally from scorched3d_events LEFT JOIN scorched3d_players ON (scorched3d_events.otherplayerid=scorched3d_players.playerid) where scorched3d_events.playerid=$playerid and eventtype=1 group by scorched3d_events.otherplayerid order by tally desc limit 25";
$result = mysql_query($query) or die("Query failed : " . mysql_error(). "<BR>query=$query");
?>
<table width="500" border="0" align="center">
<tr><td align=center><b>Top 25 Targets</b></td></tr>
</table>
<table width="500" bordercolor=#333333 cellspacing="0" cellpadding="0" border="1" align="center">
<tr>
<td bgcolor=#111111 width=50></td>
<td bgcolor=#111111><b>Target</b></td>
<td bgcolor=#111111><b>Number of times target killed</b></td>
</tr>
<?
$rownumber = 0;
while ($row = mysql_fetch_object($result))
{
	++$rownumber;
	echo "<tr>";
	echo "<td>$rownumber</td>";
	echo "<td><a href=\"playerstats.php?PlayerID=$row->killedid\">$row->name</a></td>";
	echo "<td>$row->tally</td>";
	echo "</tr>";
}
?>
</table>
<br>

<?
// Top Killers
$query=" select (scorched3d_events.playerid) as killedid, (scorched3d_players.name) as name, (count(*)) AS tally from scorched3d_events LEFT JOIN scorched3d_players ON (scorched3d_events.playerid=scorched3d_players.playerid) where otherplayerid=$playerid and eventtype=1 group by scorched3d_events.playerid order by tally desc limit 25";
$result = mysql_query($query) or die("Query failed : " . mysql_error(). "<BR>query=$query");
?>
<table width="500" border="0" align="center">
<tr><td align=center><b>Top 25 Killers</b></td></tr>
</table>
<table width="500" bordercolor=#333333 cellspacing="0" cellpadding="0" border="1" align="center">
<tr>
<td bgcolor=#111111 width=50></td>
<td bgcolor=#111111><b>Killer</b></td>
<td bgcolor=#111111><b>Number of times killed</b></td>
</tr>
<?
$rownumber=0;
while ($row = mysql_fetch_object($result))
{
	++$rownumber;
        echo "<tr>";
        echo "<td>$rownumber</td>";
        echo "<td><a href=\"playerstats.php?PlayerID=$row->killedid\">$row->name</a></td>";
        echo "<td>$row->tally</td>";
	echo "</tr>";
}
?>
</table>
<br>

<?	
// Top Weapons
$query="select (scorched3d_events.weaponid) as weaponid, (count(*)) as tally, (scorched3d_weapons.name) as name from scorched3d_events LEFT JOIN scorched3d_weapons ON (scorched3d_events.weaponid=scorched3d_weapons.weaponid) where playerid=$playerid and eventtype=1 group by weaponid order by tally desc";
$result = mysql_query($query) or die("Query failed : " . mysql_error(). "<BR>query=$query");
?>
<table width="500" border="0" align="center">
<tr><td align=center><b>Top Weapons</b></td></tr>
</table>
<table width="500" bordercolor=#333333 cellspacing="0" cellpadding="0" border="1" align="center">
<tr>
<td bgcolor=#111111 width=50></td>
<td bgcolor=#111111><b>Weapon</b></td>
<td bgcolor=#111111><b>Kills</b></td>
</tr>
<?
$rownumber=0;
while ($row = mysql_fetch_object($result))
{
	++$rownumber;
	echo "<tr>";
	echo "<td>$rownumber<br></td>";
	echo "<td><a href=\"weaponstats.php?WeaponID=$row->weaponid\">$row->name</a></td>";
	echo "<td>$row->tally<br></td>";
	echo "</tr>";
}
?>
</table>

<?
include('statsfooter.php');
?>
</body>
</html>
