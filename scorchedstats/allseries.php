<?
include('statsheader.php');
include('conversionfunctions.php')
?>

<table width="790" border="0" align="center">
<tr align=center>
<td>
<font size=+2><b><a href="overall37.php">View stats for Scorched3D - 37</a></b></font><br>
<b>Scorched3D (Player Stats, No Bots)</b><br>
<font size=-1>
2004-03-28 13:48 to 2004-12-04 00:00:00
</font>
</td>
</tr>
</table>
<br>

<?
$query=" select seriesid, rounds, games, started, ended, name  from scorched3d_series";
$seriesresult = mysqlQuery($query) or die("Query 1 failed : " . mysql_error());

while ($seriesrow = mysql_fetch_object($seriesresult))
{

$seriesid=$seriesrow->seriesid;

$totalplayersresult = mysqlQuery("select count(playerid) as playercount from scorched3d_stats where seriesid=$seriesid;") or die("Query failed : " . mysql_error());
$totalplayersrow = mysql_fetch_object($totalplayersresult);
$totalplayers = $totalplayersrow->playercount;

$query =" select prefixid, serverid from scorched3d_statssource where seriesid = ".$seriesid.";";
$outerresult = mysqlQuery($query) or die("Query 2 failed : " . mysql_error());

$server = "";
while ($outerrow = mysql_fetch_object($outerresult))
{
$serverid = $outerrow->serverid;

$serverresult = mysqlQuery("select name from scorched3d_servers where serverid = ".$serverid) or die("Query failed : " . mysql_error());
$serverrow = mysql_fetch_object($serverresult);
$server .= "<b>".$serverrow->name."</b><br>";
}

?>

<table width="790" border="0" align="center">
<tr align=center>
<td>
<font size=+2><b><a href="index.php?Series=<?=$seriesid?>">View stats for <?=$seriesrow->name?></a></b></font><br>
<?=$server?>
<font size=-1>
<?=$seriesrow->started?> to <?=$seriesrow->ended?><br>
Players : <?=$totalplayers?>, Games: <?=$seriesrow->games?>, Rounds : <?=$seriesrow->rounds?></a><br>
</font>
</td>
</tr>
</table>
<br>

<?
}
?>

<?
include('statsfooter.php');
?>
