<?
include('config.php');
                                                                                            
$link = mysql_connect($dbhost, $dbuser, $dbpasswd) or die("Could not connect : " . mysql_error());
mysql_select_db($dbname) or die("Could not select database");
?>
                                                                                            
<!DOCTYPE HTML PUBLIC "-//W3C//DTD HTML 4.01 Transitional//EN">
<html>
<head><title>Scorched3D Player Stats</title></head>
<body bgcolor="#000000" text="#FFFFFF" link="#FF9933" vlink="#FF9933" alink="#FF9933">
                                                                                            
<?
include('statsheader.php');
?>
                                                                                            
<table width=500 border="0" align="center">
<tr><td align="center">
<?
include('search.php');
?>
</td></tr>
</table>

<?
$playerid=$_GET['PlayerID'];// or die ("No player specified");
$query = " select wins, overallwinner, playerid, kills, name from scorched3d_players order by kills desc limit $playerid, 25";
$result = mysql_query($query) or die("Query failed : " . mysql_error());
?>
<table width=500 border="0" align="center">
<tr><td align=center><b>Players <? echo $playerid + 1; echo " to "; echo $playerid + 25;?></b></td></tr>
</table>
<table width=500 bordercolor=#333333 cellspacing="0" cellpadding="0" border="1" align="center">
<tr>
<td bgcolor=#111111 width=50></td>
<td bgcolor=#111111><b>Player Name</b></td>
<td bgcolor=#111111><b>Kills</b></td>
<td bgcolor=#111111><b>Round Wins</b></td>
<td bgcolor=#111111><b>Game Wins</b></td>
</tr>
<?
$rownum=0;
while ($row = mysql_fetch_object($result))
{
        ++$rownum;
        echo "<tr>";
        echo "<td>".($rownum + $playerid)."</td>";
        echo "<td><a href=\"playerstats.php?PlayerID=$row->playerid\">$row->name</a></td>";
        echo "<td>$row->kills</td>";
        echo "<td>$row->wins</td>";
        echo "<td>$row->overallwinner</td>";
        echo "</tr>";
}
?>
</table>
<table width=500 align=center border=0> 
<tr><td>
<table border=0> 
<tr>
<?
$query = "select count(playerid) from scorched3d_players;";
$result = mysql_query($query) or die("Query failed : " . mysql_error());
$row = mysql_fetch_array($result);
$pages = $row[0] / 25 ;
if (gettype($pages)!="integer") $pages++;
for ($i=1; $i<=$pages; $i++)
{
	$startplayer = ($i - 1) * 25 + 1;
	$playerindex = $startplayer - 1;
	$endplayer = $startplayer + 24;
	echo "<td align=center width=75><a href=\"allplayers.php?PlayerID=$playerindex\">[$startplayer-$endplayer]</a></td>";
}
?>
</tr>
</table>
</td></tr>
</table>


<?
include('statsfooter.php');
?>
</body>
</html>

