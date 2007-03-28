<?
$totalplayersresult = mysqlQuery("select count(playerid) as playercount from scorched3d_players") or die("Query failed : " . mysql_error());
$totalplayersrow = mysql_fetch_object($totalplayersresult);
$totalplayers = $totalplayersrow->playercount;
?>

<table width="790" border="0" align="center">
<tr align=center>
<td>
<font size=+2><b><u>No currently running stats series</u></b></font><br><br>
<a href="allseries.php"><b>View results from previous stats series</b></a><br>
<a href="weekly.php"><b>View weekly results</b></a>
</td>
</tr>
</table>

