<?
$weaponid=$_GET['WeaponID'] or die ("No Weapon specified");
include('statsheader.php');
?>

<?
// General Weapon Stats
$query = "SELECT * FROM scorched3d_weapons where weaponid=$weaponid";
$result = mysql_query($query) or die("Query failed : " . mysql_error());
$row = mysql_fetch_object($result);
$weaponname = $row->name;
?>
<table width="600" border="0" align="center">
<tr><td align=center><b><?=$weaponname?> Stats</b></td></tr>
</table>
<table width="600" bordercolor=#333333 cellspacing="0" cellpadding="0" border="1" align="center">
<tr><td bgcolor=#111111><b>Weapon Name</b></td><td><?=$row->name?></td></tr>
<tr><td bgcolor=#111111><b>Weapon ID</b></td><td><?=$row->weaponid?></td></tr>
<tr><td bgcolor=#111111><b>Total Kills</b></td><td><?=$row->kills?></td></tr>
</table>
<br>

<?
// Weapon users
$query="select (scorched3d_players.name) as name, (scorched3d_events.playerid) as killer, SUM(IF(scorched3d_events.eventtype='1',1,0)) AS kills, SUM(IF(scorched3d_events.eventtype='2',1,0)) AS teamkills , SUM(IF(scorched3d_events.eventtype='3',1,0)) AS selfkills from scorched3d_events LEFT JOIN scorched3d_players ON (scorched3d_events.playerid=scorched3d_players.playerid) WHERE weaponid=$weaponid group by killer order by kills desc limit 25";
$result = mysql_query($query) or die("Query failed : " . mysql_error(). "<BR>query=$query");
?>
<table width="600" border="0" align="center">
<tr><td align=center><b><?=$weaponname?> Users</b></td></tr>
</table>
<table width="600" bordercolor=#333333 cellspacing="0" cellpadding="0" border="1" align="center">
<tr>
<td bgcolor=#111111 width=50></td>
<td bgcolor=#111111><b>User</b></td>
<td bgcolor=#111111><b>Kills</b></td>
<td bgcolor=#111111><b>Self Kills</b></td>
<td bgcolor=#111111><b>Team Kills</b></td>
</tr>
<?
$rownumber = 0;
while ($row = mysql_fetch_object($result))
{
        ++$rownumber;
        echo "<tr>";
	echo "<td>$rownumber</td>";
	echo "<td><a href=\"playerstats.php?PlayerID=$row->killer\">$row->name</a></td>";
	echo "<td>$row->kills</td>";
	echo "<td>$row->selfkills</td>";
	echo "<td>$row->teamkills</td>";
	echo "</tr>\n";
}
?>
</table><BR>

<?
// Query last 25 weapon events
$query ="SELECT scorched3d_events.playerid, scorched3d_events.weaponid, scorched3d_events.eventtime, scorched3d_events.eventtype, scorched3d_events.otherplayerid, (playernames.name) as playername, (otherplayernames.name) as otherplayername FROM scorched3d_events LEFT JOIN scorched3d_players playernames ON scorched3d_events.playerid=playernames.playerid LEFT JOIN scorched3d_players otherplayernames ON scorched3d_events.otherplayerid=otherplayernames.playerid WHERE scorched3d_events.weaponid=$weaponid ORDER BY eventtime desc limit 25";
$result = mysql_query($query) or die("Query failed : " . mysql_error());
$row = mysql_fetch_object($result);
?>
<table width=550 border="0" align="center">
<tr><td align=center><b>Recent <?=$weaponname?> Uses</b></td></tr>
</table>
<table width="550" bordercolor=#333333 cellspacing="0" cellpadding="0" border="1" align="center">
<tr>
<td bgcolor=#111111><b>Time</b></td>
<td bgcolor=#111111><b>Player</b></td>
<td bgcolor=#111111><center><b>Event</b></center></td>
<td bgcolor=#111111><b>Other Player</b></td>
</tr>
<?
while ($row = mysql_fetch_object($result))
{
	switch ($row->eventtype)
	{
	case 1: $event = 'red>killed'; break;
	case 2: $event = 'violet>team killed'; break;
	case 3: $event = 'violet>killed self'; break;
	}
	$otherplayer = "<a href=playerstats.php?PlayerID=".$row->otherplayerid.">".$row->otherplayername."</a>";
        echo "<tr>";
        echo "<td>$row->eventtime</td>";
        echo "<td align=right><a href=playerstats.php?PlayerID=".$row->playerid.">".$row->playername."</a></td>";
        echo "<td align=center><font color=".$event."</font></td>";
        echo "<td>$otherplayer</td>";
        echo "</tr>";
}
?>
</table>

<?
include('statsfooter.php');
?>
