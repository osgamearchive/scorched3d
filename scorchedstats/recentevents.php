<?
$prefix=$_GET['Prefix'];
if ($prefix==Null)	$prefix="";
include('statsheader.php');
?>

<?
// Query player events and player/weapon names
$query ="SELECT scorched3d".$prefix."_events.playerid, scorched3d".$prefix."_events.weaponid, scorched3d".$prefix."_events.eventtime, scorched3d".$prefix."_events.eventtype, scorched3d".$prefix."_events.otherplayerid, (playernames.name) as playername, (otherplayernames.name) as otherplayername, (scorched3d".$prefix."_weapons.name) as weaponname, (scorched3d".$prefix."_weapons.armslevel) as armslevel FROM scorched3d".$prefix."_events LEFT JOIN scorched3d".$prefix."_players playernames ON scorched3d".$prefix."_events.playerid=playernames.playerid LEFT JOIN scorched3d".$prefix."_players otherplayernames ON scorched3d".$prefix."_events.otherplayerid=otherplayernames.playerid LEFT JOIN scorched3d".$prefix."_weapons ON scorched3d".$prefix."_events.weaponid=scorched3d".$prefix."_weapons.weaponid ORDER BY eventtime desc limit 100";
$result = mysql_query($query) or die("Query failed : " . mysql_error());
$row = mysql_fetch_object($result);
?>
<table width=640 border="0" align="center">
<tr><td align=center><font size="+1"><b>Recent Player Events</b></font></td></tr>
</table>
<table width="640" bordercolor=#333333 cellspacing="0" cellpadding="0" border="1" align="center">
<tr>
<td bgcolor=#111111><b>Time</b></td>
<td bgcolor=#111111><b>Player</b></td>
<td bgcolor=#111111><center><b>Event</b></center></td>
<td bgcolor=#111111><b>Other Player</b></td>
<td bgcolor=#111111><b>Weapon</b></td>
<td bgcolor=#111111><b>Armslevel</b></td>
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
	case 7: $event = 'yellow>connected'; break;
	case 8: $event = 'yellow>disconnected'; break;
	}
	$otherplayer = "<a href=playerstats.php?Prefix=".$prefix."&PlayerID=".$row->otherplayerid.">".$row->otherplayername."</a>";
	switch ($row->eventtype)
	{
	case 3: $otherplayer = "<font color=violet><center>with</center></font>"; break;
	}
        echo "<tr>";
        echo "<td>$row->eventtime</td>";
        echo "<td align=right><a href=playerstats.php?Prefix=".$prefix."&PlayerID=".$row->playerid.">".$row->playername."</a></td>";
        echo "<td align=center><font color=".$event."</font></td>";
        echo "<td>$otherplayer</td>";
        echo "<td><a href=weaponstats.php?Prefix=".$prefix."&WeaponID=".$row->weaponid.">".$row->weaponname."</a></td>";
        echo "<td>$row->armslevel</td>";
        echo "</tr>";
}
?>
</table>

<?
include('statsfooter.php');
?>
