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
$orderby=$_GET['OrderBy'];// or die ("No player specified");
if ($orderby==Null)
{
	$orderby="kills";
}
$dir=$_GET['Dir'];// or die ("No player specified");
if ($dir==Null)
{
	$dir="desc";
}
$query = " select wins, overallwinner, playerid, kills, lastconnected, shots, name, selfkills, teamkills, round(((kills-(teamkills+selfkills))/shots)*100, 1) as killratio from scorched3d_players order by $orderby $dir limit $playerid, 25";
$result = mysql_query($query) or die("Query failed : " . mysql_error());
switch ($dir)
	{
	case "asc": $oppdir = 'desc'; break;
	case "desc": $oppdir = 'asc'; break;
	}
?>
<table width=640 border="0" align="center">
<tr><td align=center><b>Players <? echo $playerid + 1; echo " to "; echo $playerid + 25;?></b></td></tr>
</table>
<table width=640 bordercolor=#333333 cellspacing="0" cellpadding="0" border="1" align="center">
<tr>
<td bgcolor=#111111 width=50></td>

<?
echo "<td bgcolor=#111111><b><a href=allplayers.php?PlayerID=0&OrderBy=name&Dir=$oppdir>Player Name</a></b></td>";
echo "<td bgcolor=#111111><b><a href=allplayers.php?PlayerID=0&OrderBy=kills&Dir=$oppdir>Kills</a></b></td>";
echo "<td bgcolor=#111111><b><a href=allplayers.php?PlayerID=0&OrderBy=killratio&Dir=$oppdir>Kill Ratio</a></b></td>";
echo "<td bgcolor=#111111><b><a href=allplayers.php?PlayerID=0&OrderBy=wins&Dir=$oppdir>Round Wins</a></b></td>";
echo "<td bgcolor=#111111><b><a href=allplayers.php?PlayerID=0&OrderBy=overallwinner&Dir=$oppdir>Game Wins</a></b></td>";
echo "<td width=160 bgcolor=#111111><b><a href=allplayers.php?PlayerID=0&OrderBy=lastconnected&Dir=$oppdir>Last Connect</b></td>";
?>
</tr>
<?
$rownum=0;
while ($row = mysql_fetch_object($result))
{
        ++$rownum;
        if ($row->shots==0)
        	$killratio = 0;
    	else
	    	$killratio = $row->killratio;
        echo "<tr>";
        echo "<td>".($rownum + $playerid)."</td>";
        echo "<td><a href=playerstats.php?PlayerID=$row->playerid>$row->name</a></td>";
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
	echo "<td align=center><font size=-2><a href=allplayers.php?PlayerID=$playerindex&OrderBy=$orderby&Dir=$dir>[$startplayer-$endplayer]</a></font></td>";
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
