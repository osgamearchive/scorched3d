<?
$playerid=$_GET['PlayerID'] or die ("No player specified");
include('statsheader.php');
?>

<?
// Query player events and player/weapon names
$query = "SELECT scorched3d_events.playerid, scorched3d_events.weaponid, scorched3d_events.eventtime, scorched3d_events.eventtype, scorched3d_events.otherplayerid, (scorched3d_players.name) as playername, (scorched3d_weapons.name) as weaponname FROM scorched3d_events LEFT JOIN scorched3d_players ON scorched3d_events.otherplayerid=scorched3d_players.playerid LEFT JOIN scorched3d_weapons ON scorched3d_events.weaponid=scorched3d_weapons.weaponid WHERE scorched3d_events.playerid=$playerid ORDER BY eventtime DESC";
$result = mysql_query($query) or die("Query failed : " . mysql_error());
$row = mysql_fetch_object($result);
?>
<table width=640 border="0" align="center">
<tr><td align=center><b>Player Events</b></td></tr>
</table>
<table width="600" bordercolor=#333333 cellspacing="0" cellpadding="0" border="1" align="center">
<tr>
<td bgcolor=#111111><b>Time</b></td>
<td bgcolor=#111111><b>Event</b></td>
<td bgcolor=#111111><b>Player</b></td>
<td bgcolor=#111111><b>Weapon</b></td>
</tr>
<?
while ($row = mysql_fetch_object($result))
{
	$event = "none";
	switch ($row->eventtype)
	{
	case 1: $event = "<font color=red>killed</font>"; break;
	case 2: $event = "team killed"; break;
	case 3: $event = "<font color=red>killed self</font>"; break;
	case 4: $event = "resigned"; break;
	case 5: $event = "won"; break;
	case 6: $event = "overall winner"; break;
	}
	$otherplayer = $row->playername;
	switch ($row->otherplayerid)
	{
	case 0: $otherplayer = " "; break;
	}
	$weapon = $row->weaponname;
	switch ($row->weaponid)
	{
	case 0: $weapon = " "; break;
	}
        echo "<tr>";
        echo "<td>$row->eventtime</td>";
        echo "<td>$event</td>";
        echo "<td>$otherplayer</td>";
        echo "<td>$weapon</td>";
        echo "</tr>";
}
?>
</table>

<?
include('statsfooter.php');
?>
