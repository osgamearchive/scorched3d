<?
include('statsheader.php');
$prefix=$_GET['Prefix'];
if ($prefix==Null)	$prefix="";
?>

<?
$query = " select ipaddress from scorched3d".$prefix."_players group by ipaddress having count(*) > 1;";
$result = mysql_query($query) or die("Query failed : " . mysql_error());
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
<td bgcolor=#111111><b>Player Name</b></td>
</tr>
<?
	$query2 = " select name, playerid from scorched3d".$prefix."_players where ipaddress=\"".$row->ipaddress."\"";
	$result2 = mysql_query($query2) or die("Query failed : " . mysql_error());

	while ($row2 = mysql_fetch_object($result2))
	{
       		echo "<tr>";
        	echo "<td><a href=\"playerstats.php?Prefix=$prefix&PlayerID=$row2->playerid\">$row2->name</a></td>";
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
