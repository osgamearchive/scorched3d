<?
$weaponid=$_GET['WeaponID'] or die ("No Weapon specified");
$prefix=$_GET['Prefix'];
if ($prefix==Null)	$prefix="";
include('statsheader.php');
include('conversionfunctions.php');
?>

<?
// General Weapon Stats
$query = "SELECT *, COALESCE(round(kills/shots, 2), 0.0) as killratio FROM scorched3d".$prefix."weapons where weaponid=$weaponid";
$result = mysql_query($query) or die("Query failed : " . mysql_error());
$row = mysql_fetch_object($result);
$weaponname=$row->name;
?>

<table width="600" border="0" align="center">
<tr><td align=center><font size="+1"><b><?=$weaponname?> Stats</b></font></td></tr>
</table>
<table width="600" bordercolor=#333333 cellspacing="0" cellpadding="2" border="1" align="center">
<tr><td bgcolor=#111111 width=100><b>Description</b></td><td><?=$row->description?></td></tr>
<tr><td bgcolor=#111111><b>Weapon ID</b></td><td><?=$row->weaponid?></td></tr>
<tr><td bgcolor=#111111><b>Armslevel</b></td><td><?=$row->armslevel?></td></tr>
<tr><td bgcolor=#111111><b>Cost</b></td><td><?=prependnumber($row->cost, "$", 0)?></td></tr>
<tr><td bgcolor=#111111><b>Bundlesize</b></td><td><?=$row->bundlesize?></td></tr>
<tr><td bgcolor=#111111><b>Total Shots</b></td><td><?=$row->shots?></td></tr>
<tr><td bgcolor=#111111><b>Total Kills</b></td><td><?=$row->kills?></td></tr>
<tr><td bgcolor=#111111><b>Kill Ratio</b></td><td>%<?=$row->killratio?></td></tr>
</table>
<br>

<?
// Weapon users
$query="select (scorched3d".$prefix."players.name) as name, (scorched3d".$prefix."events.playerid) as killer, SUM(IF(scorched3d".$prefix."events.eventtype='1',1,0)) AS kills, SUM(IF(scorched3d".$prefix."events.eventtype='2',1,0)) AS teamkills , SUM(IF(scorched3d".$prefix."events.eventtype='3',1,0)) AS selfkills from scorched3d".$prefix."events LEFT JOIN scorched3d".$prefix."players ON (scorched3d".$prefix."events.playerid=scorched3d".$prefix."players.playerid) WHERE weaponid=$weaponid group by killer order by kills desc limit 25";
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
	echo "<td><a href=\"playerstats.php?Prefix=".$prefix"&PlayerID=$row->killer\">$row->name</a></td>";
	echo "<td>$row->kills</td>";
	echo "<td>$row->selfkills</td>";
	echo "<td>$row->teamkills</td>";
	echo "</tr>\n";
}
?>
</table><BR>

<?
// Query last 25 weapon events
$query ="SELECT scorched3d".$prefix."events.playerid, scorched3d".$prefix."events.weaponid, scorched3d".$prefix."events.eventtime, scorched3d".$prefix."events.eventtype, scorched3d".$prefix."events.otherplayerid, (playernames.name) as playername, (otherplayernames.name) as otherplayername FROM scorched3d".$prefix."events LEFT JOIN scorched3d".$prefix."players playernames ON scorched3d".$prefix."events.playerid=playernames.playerid LEFT JOIN scorched3d".$prefix."players otherplayernames ON scorched3d".$prefix."events.otherplayerid=otherplayernames.playerid WHERE scorched3d".$prefix."events.weaponid=$weaponid ORDER BY eventtime desc limit 25";
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
	$otherplayer = "<a href=playerstats.php?Prefix=".$prefix"&PlayerID=".$row->otherplayerid.">".$row->otherplayername."</a>";
        echo "<tr>";
        echo "<td>$row->eventtime</td>";
        echo "<td align=right><a href=playerstats.php?Prefix=".$prefix"&PlayerID=".$row->playerid.">".$row->playername."</a></td>";
        echo "<td align=center><font color=".$event."</font></td>";
        echo "<td>$otherplayer</td>";
        echo "</tr>";
}
?>
</table>

<?
include('statsfooter.php');
?>
