<?
include('statsheader.php');
?>

<?
// Top Players Query
$query = " select osdesc, playerid, name, connects, lastconnected FROM scorched3d_players GROUP BY playerid ORDER BY lastconnected desc limit 25";
$result = mysql_query($query) or die("Query failed : " . mysql_error());
?>
<table width="640" border="0" align="center">
<tr><td align=center><b>Recent Players</b></td></tr>
</table>
<table width="600" bordercolor="#333333" cellspacing="0" cellpadding="0" border="1" align="center">
<tr>
<td bgcolor=#111111><center><b>Time</b></center></td>
<td bgcolor=#111111><center><b>Name</b></center></td>
<td bgcolor=#111111><center><b>Connects</b></center></td>
<td bgcolor=#111111><center><b>OS</b></center></td></tr>
<?
$rownum=0;	
while ($row = mysql_fetch_object($result))
{
	$tmp = preg_replace("/[\(-].*/", "", $row->osdesc);
	++$rownum;
	echo "<tr>";
	echo "<td><center>$row->lastconnected</td></center>";
	echo "<td><a href=\"playerstats.php?PlayerID=$row->playerid\">$row->name</a></td>";
	echo "<td><center>$row->connects</td></center>";
	echo "<td><center>$row->osdesc</td></center>";
	echo "</tr>";
}
?>
</table>

<?
include('statsfooter.php');
?>

