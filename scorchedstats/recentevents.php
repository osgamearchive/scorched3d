<?
include('statsheader.php');
?>

<?
// Query player events and player/weapon names
$query ="SELECT scorched3d_events.playerid, scorched3d_events.weaponid, scorched3d_events.eventtime, scorched3d_events.eventtype, scorched3d_events.otherplayerid, (playernames.name) as playername, (otherplayernames.name) as otherplayername, (scorched3d_weapons.name) as weaponname FROM scorched3d_events LEFT JOIN scorched3d_players playernames ON scorched3d_events.playerid=playernames.playerid LEFT JOIN scorched3d_players otherplayernames ON scorched3d_events.otherplayerid=otherplayernames.playerid LEFT OUTER JOIN scorched3d_weapons ON scorched3d_events.weaponid=scorched3d_weapons.weaponid ORDER BY eventtime desc limit 100";
$result = mysql_query($query) or die("Query failed : " . mysql_error());
$row = mysql_fetch_object($result);
?>
<table width=640 border="0" align="center">
<tr><td align=center><b>Recent Player Events</b></td></tr>
</table>
<table width="640" bordercolor=#333333 cellspacing="0" cellpadding="0" border="1" align="center">
<tr>
<td bgcolor=#111111><b>Time</b></td>
<td bgcolor=#111111><b>Player</b></td>
<td bgcolor=#111111><center><b>Event</b></center></td>
<td bgcolor=#111111><b>Other Player</b></td>
<td bgcolor=#111111><b>Weapon</b></td>
</tr>
<?
while ($row = mysql_fetch_object($result))
{
	$event = 'none';
	switch ($row->eventtype)
	{
	case 1: $event = '<font color=red>killed</font>'; break;
	case 2: $event = 'team killed'; break;
	case 3: $event = '<font color=red>killed self</font>'; break;
	case 4: $event = 'resigned'; break;
	case 5: $event = 'won'; break;
	case 6: $event = 'overall winner'; break;
	}
	$weapon = $row->weaponname;
	switch ($row->weaponid)
	{
	case 0: $weapon = " "; break;
	}
	$otherplayer = $row->otherplayername;
	switch ($row->eventtype)
	{
	case 3: $otherplayer = "<center>with</center>"; break;
	}
        echo "<tr>";
        echo "<td>$row->eventtime</td>";
        echo "<td align=right>$row->playername</td>";
        echo "<td align=center>$event</td>";
        echo "<td>$otherplayer</td>";
        echo "<td>$weapon</td>";
        echo "</tr>";
}
?>
</table>

<?
include('statsfooter.php');
?>
