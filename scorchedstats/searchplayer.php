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
$query = " select wins, overallwinner, playerid, kills, name from scorched3d".$prefix."_players where name rlike \"$playername\" order by kills desc";
$result = mysql_query($query) or die("Query failed : " . mysql_error());
?>
<table width=600 border="0" align="center">
<tr><td align=center><b>Players containing <? echo $playername;?></b></td></tr>
</table>
<table width=600 bordercolor=#333333 cellspacing="0" cellpadding="0" border="1" align="center">
<tr>
<td bgcolor=#111111><b>Player Name</b></td>
<td bgcolor=#111111><b>Kills</b></td>
<td bgcolor=#111111><b>Round Wins</b></td>
<td bgcolor=#111111><b>Game Wins</b></td>
</tr>
<?
while ($row = mysql_fetch_object($result))
{
        echo "<tr>";
        echo "<td><a href=\"playerstats.php?Prefix=$prefix&PlayerID=$row->playerid\">$row->name</a></td>";
        echo "<td>$row->kills</td>";
        echo "<td>$row->wins</td>";
        echo "<td>$row->overallwinner</td>";
        echo "</tr>";
}
?>
</table>

<?
include('statsfooter.php');
?>
