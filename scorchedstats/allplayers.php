<?
include('statsheader.php');
?>
                                                                                            
<table width=640 border="0" align="center">
<tr><td align="center">
<?
include('search.php');
?>
</td></tr>
</table>

<?
$playerid=$_GET['PlayerID'];// or die ("No player specified");
$query = " select wins, overallwinner, playerid, kills, lastconnected, shots, name from scorched3d_players order by kills desc limit $playerid, 25";
$result = mysql_query($query) or die("Query failed : " . mysql_error());
?>
<table width=640 border="0" align="center">
<tr><td align=center><b>Players <? echo $playerid + 1; echo " to "; echo $playerid + 25;?></b></td></tr>
</table>
<table width=640 bordercolor=#333333 cellspacing="0" cellpadding="0" border="1" align="center">
<tr>
<td bgcolor=#111111 width=50></td>
<td bgcolor=#111111><b>Player Name</b></td>
<td bgcolor=#111111><b>Kills</b></td>
<td bgcolor=#111111><b>Kill Ratio</b></td>
<td bgcolor=#111111><b>Round Wins</b></td>
<td bgcolor=#111111><b>Game Wins</b></td>
<td width=160 bgcolor=#111111><b>Last Connect</b></td>
</tr>
<?
$rownum=0;
while ($row = mysql_fetch_object($result))
{
        ++$rownum;
        $killratio = round($row->kills/$row->shots, 3)*100;
        echo "<tr>";
        echo "<td>".($rownum + $playerid)."</td>";
        echo "<td><a href=\"playerstats.php?PlayerID=$row->playerid\">$row->name</a></td>";
        echo "<td>$row->kills</td>";
     	echo "<td>$killratio%</td>";
        echo "<td>$row->wins</td>";
        echo "<td>$row->overallwinner</td>";
        echo "<td>$row->lastconnected</td>";
        echo "</tr>";
}
?>
</table>
<table width=640 align=center border=0> 
<tr><td>
<table border=0 align=center> 
<tr>
<?
$query = "select count(playerid) from scorched3d_players;";
$result = mysql_query($query) or die("Query failed : " . mysql_error());
$row = mysql_fetch_array($result);
$pages = $row[0] / 25 ;
$rows = 0;
if (gettype($pages)!="integer") $pages++;
for ($i=1; $i<=$pages; $i++)
{
	$startplayer = ($i - 1) * 25 + 1;
	$playerindex = $startplayer - 1;
	$endplayer = $startplayer + 24;
	echo "<td align=center><font size=-2><a href=\"allplayers.php?PlayerID=$playerindex\">[$startplayer-$endplayer]</a></font></td>";
	$rows++;
	if ($rows > 7)
	{
		$rows = 0;
		echo "</tr><tr>\n";
	}
}
?>
</tr>
</table>
</td></tr>
</table>

<?
include('statsfooter.php');
?>
