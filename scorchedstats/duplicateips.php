<?
include('statsheader.php');
?>

<?
$query = " select ipaddress from scorched3d_players group by ipaddress having count(*) > 1;";
$result = mysqlQuery($query) or die("Query failed : " . mysql_error());
?>

<table width=600 border="0" align="center">
<tr><td align=center><b>Players with duplicate ip addresses</b></td></tr>
</table>

<?
while ($row = mysql_fetch_object($result))
{
?>
<table width=600 bordercolor=#333333 cellspacing="0" cellpadding="0" border="1" align="center">
<tr>
<td bgcolor=#111111><b>Player Id</b></td>
<td bgcolor=#111111><b>Player Name</b></td>
</tr>
<?
	$query2 = " select name, playerid from scorched3d_players where ipaddress=\"".$row->ipaddress."\"";
	$result2 = mysqlQuery($query2) or die("Query failed : " . mysql_error());

	while ($row2 = mysql_fetch_object($result2))
	{
       		echo "<tr>";
        	echo "<td>$row2->playerid</td>";
        	echo "<td>$row2->name</td>";
        	echo "</tr>\n";
	}
?>
</table><br>
<?
}
?>

<?
include('statsfooter.php');
?>
