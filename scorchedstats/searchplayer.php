<?
include('statsheader.php');
$prefix=$_GET['Prefix'];
if ($prefix==Null)	$prefix="";
?>

<table width=600 border="0" align="center">
<tr><td align="center">
<?
include('search.php');
?>
</td></tr>
</table>

<?
$playername=$_GET['playername'] or die ("No player name specified");
$query = " select name, playerid from scorched3d".$prefix."_names where name rlike \"$playername\"";
$result = mysql_query($query) or die("Query failed : " . mysql_error());
?>
<table width=600 border="0" align="center">
<tr><td align=center><b>Players containing <? echo $playername;?></b></td></tr>
</table>
<table width=600 bordercolor=#333333 cellspacing="0" cellpadding="0" border="1" align="center">
<tr>
<td bgcolor=#111111><b>Player Name</b></td>
<td bgcolor=#111111><b>Player ID</b></td>
<td bgcolor=#111111><b>Last Used Player Name</b></td>
</tr>
<?
while ($row = mysql_fetch_object($result))
{
	$query2 = " select name from scorched3d".$prefix."_players where playerid=\"".$row->playerid."\"";
	$result2 = mysql_query($query2) or die("Query failed : " . mysql_error());
	$row2 = mysql_fetch_object($result2);

        echo "<tr>";
        echo "<td><a href=\"playerstats.php?Prefix=$prefix&PlayerID=$row->playerid\">$row->name</a></td>";
	echo "<td>".$row->playerid."</td>";
	echo "<td>".$row2->name."</td>";
        echo "</tr>\n";
}
?>
</table>

<?
include('statsfooter.php');
?>
