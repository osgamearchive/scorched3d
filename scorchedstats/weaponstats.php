<?
$weaponid=$_GET['WeaponID'] or die ("No Weapon specified");
include('statsheader.php');
?>

<?
// General Weapon Stats
$query = "SELECT * FROM scorched3d_weapons where weaponid=$weaponid";
$result = mysql_query($query) or die("Query failed : " . mysql_error());
$row = mysql_fetch_object($result);
?>
<table width="600" border="0" align="center">
<tr><td align=center><b>Weapon Stats</b></td></tr>
</table>
<table width="600" bordercolor=#333333 cellspacing="0" cellpadding="0" border="1" align="center">
<tr><td bgcolor=#111111><b>Weapon Name</b></td><td><?=$row->name?></td></tr>
<tr><td bgcolor=#111111><b>Weapon ID</b></td><td><?=$row->weaponid?></td></tr>
<tr><td bgcolor=#111111><b>Total Kills</b></td><td><?=$row->kills?></td></tr>
</table>
<br>

<?
// Weapon users
$query="select (scorched3d_players.name) as name, (scorched3d_events.playerid) as killer, (count(*)) as tally from scorched3d_events LEFT JOIN scorched3d_players ON (scorched3d_events.playerid=scorched3d_players.playerid) where weaponid=$weaponid and eventtype=1 group by killer order by tally desc limit 25";
$result = mysql_query($query) or die("Query failed : " . mysql_error(). "<BR>query=$query");
?>
<table width="600" border="0" align="center">
<tr><td align=center><b>Weapon Users</b></td></tr>
</table>
<table width="600" bordercolor=#333333 cellspacing="0" cellpadding="0" border="1" align="center">
<tr>
<td bgcolor=#111111 width=50></td>
<td bgcolor=#111111><b>User</b></td>
<td bgcolor=#111111><b>Kills</b></td>
</tr>
<?
$rownumber = 0;
while ($row = mysql_fetch_object($result))
{
        ++$rownumber;
        echo "<tr>";
	echo "<td>$rownumber</td>";
	echo "<td><a href=\"playerstats.php?PlayerID=$row->killer\">$row->name</a></td>";
	echo "<td>$row->tally</td>";
	echo "</tr>\n";
}
?>
</table>

<?
include('statsfooter.php');
?>
