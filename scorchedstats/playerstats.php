<?
include('statsheader.php');
include('conversionfunctions.php');
include('medals.php');

$prefixid = getIntParameter('Prefix');
$seriesid = getIntParameter('Series');
$playerid = getIntParameter('PlayerID');

// General Player Stats
$playerquery = "SELECT playerid, osdesc, name, avatarid FROM scorched3d_players WHERE playerid=$playerid";
$playerresult = mysqlQuery($playerquery) or die("Query failed : " . mysql_error());
if (!($playerrow = mysql_fetch_object($playerresult)))
{
	die("Failed to find a player for id");
}
$playername=$playerrow->name;
?>

<table width="790" border="0" align="center">
<tr align=center>
<td>
<font size=+2><b><u><?=$playername?>'s User Info</u></b></font><br>
</td>
</tr>
</table>
<br>

<table width=640 border="0" align="center">
<tr><td align=center><b>Info</b></td></tr>
</table>
<table width="600" bordercolor=#333333 cellspacing="0" cellpadding="0" border="1" align="center">
<tr><td bgcolor=#111111><b>Player Icon</b></td><td><img src='<?=getBinaryImage($playerrow->avatarid)?>'></td></tr>
<tr><td bgcolor=#111111>
<b>Last Used Player Name</b></td><td><?=$playerrow->name?> 
<a href="playernames.php?Prefix=<?=$prefixid?>&Series=<?=$seriesid?>&PlayerID=<?=$playerid?>">(View all names)</a></td></tr>
<tr><td bgcolor=#111111><b>OS</b></td><td><?=$playerrow->osdesc?></td></tr>
<tr><td bgcolor=#111111><b>Player ID</b></td><td><?=$playerrow->playerid?></td></tr>
<tr><td bgcolor=#111111><b>Medals</b></td><td>
<?
$seriesquery = "SELECT * FROM scorched3d_stats ".
	"LEFT JOIN scorched3d_series ON scorched3d_series.seriesid = scorched3d_stats.seriesid ".
	"LEFT JOIN scorched3d_prefixs ON scorched3d_prefixs.prefixid = scorched3d_stats.prefixid ".
	"WHERE playerid = ".$playerid." AND type = 1 ORDER BY rank";
$seriesresult = mysqlQuery($seriesquery) or die("Query failed : " . mysql_error());
while ($seriesrow = mysql_fetch_object($seriesresult))
{
	$medal = get_medal($playerid, $seriesrow->rank, 0, $seriesrow->prefixid, 
		$seriesrow->seriesid, $seriesrow->name . " (". $seriesrow->prefix . ")");
	print $medal;
}
?>
</td></tr>
<tr><td bgcolor=#111111><b>Current Placings</b></td><td>
<?
$seriesquery = "SELECT * FROM scorched3d_stats ".
	"LEFT JOIN scorched3d_series ON scorched3d_series.seriesid = scorched3d_stats.seriesid ".
	"LEFT JOIN scorched3d_prefixs ON scorched3d_prefixs.prefixid = scorched3d_stats.prefixid ".
	"WHERE playerid = ".$playerid." AND type = 0 ORDER BY rank";
$seriesresult = mysqlQuery($seriesquery) or die("Query failed : " . mysql_error());
while ($seriesrow = mysql_fetch_object($seriesresult))
{
?>
Currently placed "<?=$seriesrow->rank?> in 
<a href="playerstats.php?Prefix=<?=$seriesrow->prefixid?>&
Series=<?=$seriesrow->seriesid?>&PlayerID=<?=$playerid?>">
<?=$seriesrow->name?> (<?=$seriesrow->prefix?>)</a></br>
<?
}
?>
</td></tr>
</table>
<br>

<?
// Main Stats
$query = "SELECT *, round((skill + (overallwinner*5) + (COALESCE(round((wins/gamesplayed)*(skill-1000), 3), 0)) + ((skill-1000) * COALESCE(round(((kills-(teamkills+selfkills))/shots), 3), 0))),0) as skill FROM scorched3d_stats WHERE playerid=$playerid AND prefixid=$prefixid AND seriesid=$seriesid GROUP BY playerid ORDER BY kills";
$result = mysqlQuery($query) or die("Query failed : " . mysql_error());
if (!($row = mysql_fetch_object($result))) die("Invalid player");

if ($row->shots==0)	$killratio="No Shots Recorded";
else $killratio=prependnumber(((($row->kills-$row->teamkills-$row->selfkills)/$row->shots)*100), "%", 2);
if ($row->kills==0){
	$shotsperkill=0;
	$cashperkill=0;
}
else{
	$shotsperkill=round($row->shots/$row->kills, 2);
	$cashperkill=round($row->moneyearned/$row->kills, 2);
}
if ($row->gamesplayed==0)
	$killsperround="No Rounds Recorded";
else
	$killsperround = round((($row->kills-$row->teamkills-$row->selfkills)/$row->gamesplayed), 2);	
?>

<?include('util.php');?>

<table width=640 border="0" align="center">
<tr><td align=center><b>Main Stats</b></td></tr>
</table>
<table width="600" bordercolor=#333333 cellspacing="0" cellpadding="0" border="1" align="center">

<tr><td bgcolor=#111111><b>Rank</b></td><td><?echo ($row->rank)?></td></tr>
<tr><td bgcolor=#111111><b>Last Connected</b></td><td><?=$row->lastconnected?></td></tr>
<tr><td bgcolor=#111111><b>Times Connected</b></td><td><?=$row->connects?></td></tr>
<tr><td bgcolor=#111111><b>Time Played</b></td><td><? echo secondstotext($row->timeplayed)."  (".number_format($row->timeplayed, 0)." seconds)";?></td></tr>
<tr><td bgcolor=#111111><b>Games Won</b></td><td><?=$row->overallwinner?></td></tr>
<tr><td bgcolor=#111111><b>Rounds Played</b></td><td><?=$row->gamesplayed?></td></tr>
<tr><td bgcolor=#111111><b>Rounds Won</b></td><td><?=$row->wins?></td></tr>
<tr><td bgcolor=#111111><b>Rounds Resigned</b></td><td><?=$row->resigns?></td></tr>
<tr><td bgcolor=#111111><b>Total Turns</b></td><td><?=$row->roundsplayed?></td></tr>
<tr><td bgcolor=#111111><b>Total Shots</b></td><td><?=$row->shots?></td></tr>
<tr><td bgcolor=#111111><b>Total Score</b></td><td><?=$row->scoreearned?></td></tr>
<tr><td bgcolor=#111111><b>Skill Rating</b></td><td><?=$row->skill?></td></tr>
<tr><td bgcolor=#111111><b>Opponent Kills</b></td><td><?=$row->kills?></td></tr>
<tr><td bgcolor=#111111><b>Kill Ratio</b></td><td><?echo $killratio."  (".$shotsperkill;?> shots/kill)</td></tr>
<tr><td bgcolor=#111111><b>Kills Per Round</b></td><td><?=$killsperround?></td></tr>
<tr><td bgcolor=#111111><b>Money Earned</b></td><td><?=prependnumber($row->moneyearned, "$", 0)?></td></tr>
<tr><td bgcolor=#111111><b>Money Per Kill</b></td><td><?=prependnumber($cashperkill, "$", 2)?></td></tr>
<tr><td bgcolor=#111111><b>Self Kills</b></td><td><?=$row->selfkills?></td></tr>
<tr><td bgcolor=#111111><b>Team Kills</b></td><td><?=$row->teamkills?></td></tr>
<tr><td bgcolor=#111111><b>Deaths</b></td><td><?=$row->deaths?></td></tr>
</table>
<table width="600" border="0" align="center">
<tr><td><a href="playerrecentevents.php?Prefix=<?=$prefixid?>&Series=<?=$seriesid?>&PlayerID=<?=$playerid?>">
View stats calculated from server events.</a></td></tr>
</table>
<br>

<?
include('statsfooter.php');
?>
