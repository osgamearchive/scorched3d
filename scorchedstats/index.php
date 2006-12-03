<?
include('statsheader.php');
include('conversionfunctions.php')
?>

<?
$seriesid = getIntParameter('Series');
if ($seriesid==0)
{
	// Series
	$query=" select seriesid, rounds, games, started, ended, name from scorched3d_series where type = 0;";
	$result = mysqlQuery($query) or die("Query 1 failed : " . mysql_error());
	$seriesrow = mysql_fetch_object($result);
	$seriesid = $seriesrow->seriesid;
}
else
{
	$query=" select seriesid, rounds, games, started, ended, name  from scorched3d_series where seriesid = ".$seriesid;
	$result = mysqlQuery($query) or die("Query 1 failed : " . mysql_error());
	$seriesrow = mysql_fetch_object($result);
}

$totalplayersresult = mysqlQuery("select count(playerid) as playercount from scorched3d_players") or die("Query failed : " . mysql_error());
$totalplayersrow = mysql_fetch_object($totalplayersresult);
$totalplayers = $totalplayersrow->playercount;
?>

<table width="790" border="0" align="center">
<tr align=center>
<td>
<font size=+2><b><u>Stats for <?=$seriesrow->name?></u></b></font><br>
<font size=-1>
<?=$seriesrow->started?> to <?=$seriesrow->ended?><br>
Players : <?=$totalplayers?>, Games: <?=$seriesrow->games?>, Rounds : <?=$seriesrow->rounds?>, Server : <a href="playeros.php">activity</a><br>
<a href="allseries.php">View stats for other versions of Scorched3D</a><br>
<a href="weekly.php">View weekly stats</a>
</font>
</td>
</tr>
</table>

<?
// Prefixs
$query =" select prefixid, serverid from scorched3d_statssource where seriesid = ".$seriesid.";";
$outerresult = mysqlQuery($query) or die("Query 2 failed : " . mysql_error());
?>

<?
// For each prefix in this series
while ($outerrow = mysql_fetch_object($outerresult))
{
$prefixid = $outerrow->prefixid;
$serverid = $outerrow->serverid;
?>

<?
$countresult = mysqlQuery("select count(playerid) as playercount from scorched3d_stats where prefixid = ".$prefixid." and seriesid = ".$seriesid) or die("Query failed : " . mysql_error());
$countrow = mysql_fetch_object($countresult);
$count = $countrow->playercount;

$serverresult = mysqlQuery("select name, displaystats from scorched3d_servers where serverid = ".$serverid) or die("Query failed : " . mysql_error());
$serverrow = mysql_fetch_object($serverresult);
$server = $serverrow->name;

if ($serverrow->displaystats == 0) continue;
?>

<br><br>
<table width="790" border="0" align="center">
<tr>
<td align=Center>
<b><font size="+1">Stats for server : <? echo $server; ?></font></b>
</td>
</tr>

<tr>
<td align=center>
<table align=center border=0>
<tr>
<td><b><font size=-1><a href="allstats.php?Prefix=<?=$prefixid?>&Series=<?=$seriesid?>&Fields=skl:k:me:w:ow:r:sk:tk:d:tp&OrderBy=kills">Rankings</a></font></b></td>
<td align=center width=20>|</td>
<td><b><font size=-1><a href="allstats.php?Prefix=<?=$prefixid?>&Series=<?=$seriesid?>&Fields=skl:k:ow:kr:wr:mpk:mph:tp&OrderBy=skill">Extended Rankings</a></font></b></td>
<td align=center width=20>|</td>
<td><b><font size=-1><a href="allweapons.php?Prefix=<?=$prefixid?>&Series=<?=$seriesid?>">Weapon Stats</a></font></b><br></td>
<td align=center width=20>|</td>
<td><b><font size=-1><a href="recentevents.php?Prefix=<?=$prefixid?>&Series=<?=$seriesid?>">Recent Events</a></font></b><br></td>
<td align=center width=20>|</td>
<td><b><font size=-1><a href="recentplayers.php?Prefix=<?=$prefixid?>&Series=<?=$seriesid?>">Recent Players</a></font></b><br></td>
<td align=center width=20>|</td>
<td><b><font size=-1><a href="stathistory.php?Prefix=<?=$prefixid?>&Series=<?=$seriesid?>">History</a></font></b><br></td>
</tr>
</table>
</td>
</tr>

<tr>
<td>
<?
// Top Players Query
$query = "select *, name, avatarid, round((skill + (overallwinner*5) + (COALESCE(round((wins/gamesplayed)*(skill-1000), 3), 0)) + ((skill-1000) * COALESCE(round(((kills-(teamkills+selfkills))/shots), 3), 0))),0) as skill from scorched3d_stats LEFT JOIN scorched3d_players playernames ON scorched3d_stats.playerid=playernames.playerid where prefixid = ".$prefixid." and seriesid = ".$seriesid." order by kills desc limit 10";
$result = mysqlQuery($query) or die("Query failed : " . mysql_error());
?>

<table width="650" bordercolor="#333333" cellspacing="0" cellpadding="0" border="1" align="center">
<tr>
<td bgcolor=#111111 width=50></td>
<td bgcolor=#111111><b><a href="allstats.php?Prefix=<?=$prefixid?>&Series=<?=$seriesid?>&Fields=skl:k:me:w:ow:r:sk:tk:d:tp&OrderBy=name&Dir=asc">Player Name</a></b></td>
<td bgcolor=#111111 align=center><b><a href="allstats.php?Prefix=<?=$prefixid?>&Series=<?=$seriesid?>&Fields=skl:k:me:w:ow:r:sk:tk:d:tp&OrderBy=skill">Skill</a></b></td>
<td bgcolor=#111111 align=center><b><a href="allstats.php?Prefix=<?=$prefixid?>&Series=<?=$seriesid?>&Fields=skl:k:me:w:ow:r:sk:tk:d:tp&OrderBy=kills">Kills</a></b></td>
<td bgcolor=#111111 align=center><b><a href="allstats.php?Prefix=<?=$prefixid?>&Series=<?=$seriesid?>&Fields=skl:k:me:w:ow:r:sk:tk:d:tp&OrderBy=moneyearned">Money</a></b></td>
<td bgcolor=#111111 align=center><b><a href="allstats.php?Prefix=<?=$prefixid?>&Series=<?=$seriesid?>&Fields=skl:k:me:w:ow:r:sk:tk:d:tp&OrderBy=wins">Round Wins</a></b></td>
<td bgcolor=#111111 align=center><b><a href="allstats.php?Prefix=<?=$prefixid?>&Series=<?=$seriesid?>&Fields=skl:k:me:w:ow:r:sk:tk:d:tp&OrderBy=overallwinner">Game Wins</a></b></td>
<td bgcolor=#111111 align=center><center><b>Last Connected</b></center></td>
</tr>
<?
$rownum=0;	
while ($row = mysql_fetch_object($result))
{
	++$rownum;
	echo "<tr>";
	echo "<td align=center>$rownum</center></td>";
	echo "<td>";
	echo "<table><tr>";
	echo "<td align=center><img border=0 src='getbinary.php?id=".$row->avatarid."'></td>";
	echo "<td>";
	echo "<a href=\"playerstats.php?Prefix=".$prefixid."&Series=".$seriesid."&PlayerID=".$row->playerid."\">";
	echo $row->name;
	echo "</a>";
	echo "</td>";
	echo "</tr></table>";
	echo "</td>";
	echo "<td align=center>$row->skill</td>";
	echo "<td align=center>$row->kills</td>";
	echo "<td align=center>".prependnumber($row->moneyearned, "$", 0)."</td>";
	echo "<td align=center>$row->wins</td>";
	echo "<td align=center>$row->overallwinner</td>";
	echo "<td align=right>$row->lastconnected</td>";
	echo "</tr>";
}
?>
</table>

</tr>
</td>
<tr><td align=center>
You can sort all <? echo $count; ?> players by <a href="allstats.php?Prefix=<?=$prefixid?>&Series=<?=$seriesid?>&Fields=skl:k:me:w:ow:r:sk:tk:d:tp&OrderBy=resigns">resigns</a>, <a href="allstats.php?Prefix=<?=$prefixid?>&Series=<?=$seriesid?>&Fields=skl:k:me:w:ow:r:sk:tk:d:tp&OrderBy=selfkills">suicides</a> and <a href="allstats.php?Prefix=<?=$prefixid?>&Series=<?=$seriesid?>&Fields=skl:k:me:w:ow:r:sk:tk:d:tp&OrderBy=teamkills">teamkills</a> as well.
<br>
</tr></td>
</table>
<?
}
?>

<p align=center>
<?
include('search.php');
?>
</p>

<?
include('statsfooter.php');
?>
