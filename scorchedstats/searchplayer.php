<?
include('statsheader.php');
?>

<table width=600 border="0" align="center">
<tr><td align="center">
<?
include('search.php');
?>
</td></tr>
</table>

<?
$playername = ( isset($HTTP_GET_VARS['playername']) ) ? htmlspecialchars($HTTP_GET_VARS['playername']) : '';
$playername = str_replace("\'", "''", $playername);

$query = " select name, playerid from scorched3d_names where name rlike '$playername'";
$result = mysqlQuery($query) or die("Query failed : " . mysql_error());
?>
<table width=600 border="0" align="center">
<tr><td align=center><b>Players containing <? echo $playername;?></b></td></tr>
</table>
<table width=600 bordercolor=#333333 cellspacing="0" cellpadding="0" border="1" align="center">
<tr>
<td bgcolor=#111111><b>Player Name</b></td>
<td bgcolor=#111111><b>Player ID</b></td>
<td bgcolor=#111111><b>Last Used Player Name</b></td>
<td bgcolor=#111111><b>View Stats Page</b></td>
</tr>
<?
while ($row = mysql_fetch_object($result))
{
	$query2 = " select name from scorched3d_players where playerid=\"".$row->playerid."\"";
	$result2 = mysqlQuery($query2) or die("Query failed : " . mysql_error());
	$row2 = mysql_fetch_object($result2);

	$games = "";
	$query3 = " select seriesid, prefixid from scorched3d_stats where playerid=\"".$row->playerid."\"";
	$result3 = mysqlQuery($query3) or die("Query failed : " . mysql_error());
	while ($row3 = mysql_fetch_object($result3))
	{
		$games .= "<a href='playerstats.php?Prefix=".$row3->prefixid."&Series=".$row3->seriesid."&PlayerID=".$row->playerid."'>View</a> ";
	}

        echo "<tr>";
        echo "<td>".$row->name."</td>";
	echo "<td>".$row->playerid."</td>";
	echo "<td>".$row2->name."</td>";
	echo "<td>".$games."</td>";
        echo "</tr>\n";
}
?>
</table>

<?
include('statsfooter.php');
?>
