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
$playername=$_GET['playername'] or die ("No player name specified");
$query = " select wins, overallwinner, playerid, kills, name from scorched3d_players where name rlike \"$playername\" order by kills desc";
$result = mysql_query($query) or die("Query failed : " . mysql_error());
?>
<table width=500 border="0" align="center">
<tr><td align=center><b>Players containing <? echo $playername;?></b></td></tr>
</table>
<table width=500 bordercolor=#333333 cellspacing="0" cellpadding="0" border="1" align="center">
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
        echo "<td><a href=\"playerstats.php?PlayerID=$row->playerid\">$row->name</a></td>";
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
</body>
</html>

