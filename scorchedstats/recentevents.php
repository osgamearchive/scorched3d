<?
include('statsheader.php');

$prefixid = getIntParameter('Prefix');
$seriesid = getIntParameter('Series');
?>

<? include('util.php'); ?>
<br>

<?
// Query player events and player/weapon names
$query ="SELECT scorched3d_events.playerid, scorched3d_events.weaponid, scorched3d_events.eventtime, scorched3d_events.eventtype, scorched3d_events.otherplayerid, (playernames.name) as playername, (otherplayernames.name) as otherplayername, (playernames.avatarid) as avatarid, (otherplayernames.avatarid) as otheravatarid, (scorched3d_weapons.name) as weaponname, (scorched3d_weapons.armslevel) as armslevel FROM scorched3d_events LEFT JOIN scorched3d_players playernames ON scorched3d_events.playerid=playernames.playerid LEFT JOIN scorched3d_players otherplayernames ON scorched3d_events.otherplayerid=otherplayernames.playerid LEFT JOIN scorched3d_weapons ON scorched3d_events.weaponid=scorched3d_weapons.weaponid where scorched3d_events.prefixid=$prefixid and scorched3d_events.seriesid=$seriesid ORDER BY eventtime desc limit 100";
$result = mysqlQuery($query) or die("Query failed : " . mysql_error());
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
	
	switch ($row->eventtype)
	{
	case 1:
	case 2: $otherplayer = "<table><tr><td align=center><img border=0 src='getbinary.php?id=".$row->otheravatarid."'></td><td><a href=playerstats.php?Prefix=".$prefixid."&Series=".$seriesid."&PlayerID=".$row->otherplayerid.">".$row->otherplayername."</a></td></tr></table>"; break;
	case 3: $otherplayer = "<font color=violet><center>with</center></font>"; break;
	case 4:
	case 5:
	case 6:
	case 7:
	case 8: $otherplayer = ''; break;
	}
        echo "<tr>";
        echo "<td>$row->eventtime</td>";
        echo "<td>";
		echo "<table><tr>";
		echo "<td align=center><img border=0 src='getbinary.php?id=".$row->avatarid."'></td>";
		echo "<td><a href=playerstats.php?Prefix=".$prefixid."&Series=".$seriesid."&PlayerID=$row->playerid>$row->playername</a></td>";
		echo "</td>";
		echo "</tr></table>";
        echo "<td align=center><font color=".$event."</font></td>";
        echo "<td>$otherplayer</td>";
        echo "<td><a href=weaponstats.php?Prefix=".$prefixid."&Series=".$seriesid."&WeaponID=".$row->weaponid.">".$row->weaponname."</a></td>";
        echo "<td>$row->armslevel</td>";
        echo "</tr>";
}
?>
</table>

<?
include('statsfooter.php');
?>
