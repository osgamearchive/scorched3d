<?
$playerid=$_GET['PlayerID'] or die ("No player specified");
include('statsheader.php');
?>

<?
// Query player events and player/weapon names
$query = "SELECT scorched3d_events.playerid, scorched3d_events.weaponid, scorched3d_events.eventtime, scorched3d_events.eventtype, scorched3d_events.otherplayerid, (scorched3d_players.name) as playername, (scorched3d_weapons.name) as weaponname FROM scorched3d_events LEFT JOIN scorched3d_players ON scorched3d_events.otherplayerid=scorched3d_players.playerid LEFT JOIN scorched3d_weapons ON scorched3d_events.weaponid=scorched3d_weapons.weaponid WHERE (scorched3d_events.playerid=$playerid) ORDER BY eventtime DESC";
$result = mysql_query($query) or die("Query failed : " . mysql_error());
$row = mysql_fetch_object($result);
?>
<table width=640 border="0" align="center">
<tr><td align=center><b>Player Events</b></td></tr>
</table>
<table width="600" bordercolor=#333333 cellspacing="0" cellpadding="0" border="1" align="center">
<tr>
<td bgcolor=#111111><b>Time</b></td>
<td bgcolor=#111111><b><center>Event</center></b></td>
<td bgcolor=#111111><b>Player</b></td>
<td bgcolor=#111111><b>Weapon</b></td>
</tr>
<?
while ($row = mysql_fetch_object($result))
{
	switch ($row->eventtype)
	{
	case 1: $event = 'red>killed'; break;
	case 2: $event = 'violet>team killed'; break;
	case 3: $event = 'violet>killed self'; break;
	case 4: $event = 'yellow>resigned'; break;
	case 5: $event = 'green>won'; break;
	case 6: $event = 'blue>overall winner'; break;
	}
	$otherplayer = "<a href=playerstats.php?PlayerID=".$row->otherplayerid.">".$row->playername;
	switch ($row->eventtype)
	{
	case 3: $otherplayer = "<font color=violet><center>with</center></font>"; break;
	}
        echo "<tr>";
        echo "<td>$row->eventtime</td>";
        echo "<td align=center><font color=".$event."</font></td>";
        echo "<td>$otherplayer</a>";
        echo "<td><a href=weaponstats.php?WeaponID=".$row->weaponid.">".$row->weaponname."</a></td>";
        echo "</tr>";
}
?>
</table>

<?
include('statsfooter.php');
?>
