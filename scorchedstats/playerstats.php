<?
$playerid=$_GET['PlayerID'] or die ("No player specified");
$prefixid=$_GET['Prefix'] or die ("No prefix specified"); 
$seriesid=$_GET['Series'] or die ("No series specified"); 

include('statsheader.php');
include('conversionfunctions.php');

// General Player Stats

$query = "SELECT *, round((skill + (overallwinner*5) + (COALESCE(round((wins/gamesplayed)*(skill-1000), 3), 0)) + ((skill-1000) * COALESCE(round(((kills-(teamkills+selfkills))/shots), 3), 0))),0) as skill FROM scorched3d_stats WHERE playerid=$playerid AND prefixid=$prefixid AND seriesid=$seriesid GROUP BY playerid ORDER BY kills";
$result = mysql_query($query) or die("Query failed : " . mysql_error());
$row = mysql_fetch_object($result);

$playerquery = "SELECT * FROM scorched3d_players WHERE playerid=$playerid";
$playerresult = mysql_query($playerquery) or die("Query failed : " . mysql_error());
$playerrow = mysql_fetch_object($playerresult);
$playername=$playerrow->name;

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
<table width=640 border="0" align="center">
<tr><td align=center><b><?=$playername?>'s Info</b></td></tr>
</table>
<table width="600" bordercolor=#333333 cellspacing="0" cellpadding="0" border="1" align="center">
<tr><td bgcolor=#111111><b>Player Icon</b></td><td><img src="getbinary.php?id=<?=$playerrow->avatarid?>"></td></tr>
<tr><td bgcolor=#111111><b>Last Used Player Name</b></td><td><?=$playerrow->name?></td></tr>
<tr><td bgcolor=#111111><b>OS</b></td><td><?=$playerrow->osdesc?></td></tr>
<tr><td bgcolor=#111111><b>Player ID</b></td><td><?=$playerrow->playerid?></td></tr>
</table>
<br>

<table width=640 border="0" align="center">
<tr><td align=center><b><?=$playername?>'s Stats</b></td></tr>
</table>
<table width="600" bordercolor=#333333 cellspacing="0" cellpadding="0" border="1" align="center">
<tr><td bgcolor=#111111><b>Last Connected</b></td><td><?=$row->lastconnected?></td></tr>
<tr><td bgcolor=#111111><b>Times Connected</b></td><td><?=$row->connects?></td></tr>
<tr><td bgcolor=#111111><b>Time Played</b></td><td><? echo secondstotext($row->timeplayed)."  (".number_format($row->timeplayed, 0)." seconds)";?></td></tr>
<tr><td bgcolor=#111111><b>Games Won</b></td><td><?=$row->overallwinner?></td></tr>
<tr><td bgcolor=#111111><b>Rounds Played</b></td><td><?=$row->gamesplayed?></td></tr>
<tr><td bgcolor=#111111><b>Rounds Won</b></td><td><?=$row->wins?></td></tr>
<tr><td bgcolor=#111111><b>Rounds Resigned</b></td><td><?=$row->resigns?></td></tr>
<tr><td bgcolor=#111111><b>Total Turns</b></td><td><?=$row->roundsplayed?></td></tr>
<tr><td bgcolor=#111111><b>Total Shots</b></td><td><?=$row->shots?></td></tr>
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
<tr><td><a href="playerevents.php?Prefix=<?=$prefixid?>&Series=<?=$seriesid?>&PlayerID=<?=$playerid?>">
View all this players events.</a></td></tr>
</table>
<br>

<?
// Player Aliases
$query = "SELECT * FROM scorched3d_names where playerid=$playerid";
$result = mysql_query($query) or die("Query failed : " . mysql_error());
?>
<table width="600" border="0" align="center">
<tr><td align=center><b><?=$playername?>'s Aliases</b></td></tr>
</table>
<table width="600" bordercolor=#333333 cellspacing="0" cellpadding="0" border="1" align="center">
<tr>
<td bgcolor=#111111><b>Player Alias</b></td>
<td bgcolor=#111111><b>Times Used</b></td>
</tr>
<?
	while ($row = mysql_fetch_object($result))
	{
		echo "<tr><td>$row->name</td><td>$row->count</td></tr>\n";
	}
?>
</table>
<br>

<?
// Query player's stats for today
$query = "select (scorched3d_events.playerid) as playerid, (scorched3d_players.name) as name, SUM(IF(scorched3d_events.eventtype='1',1,0)) AS kills, SUM(IF(scorched3d_events.eventtype='2',1,0)) AS teamkills, SUM(IF(scorched3d_events.eventtype='3',1,0)) AS selfkills, SUM(IF(scorched3d_events.eventtype='4',1,0)) AS resigns, SUM(IF(scorched3d_events.eventtype='5',1,0)) AS wins, SUM(IF(scorched3d_events.eventtype='6',1,0)) AS gamewins FROM scorched3d_events LEFT JOIN scorched3d_players ON (scorched3d_events.playerid=scorched3d_players.playerid) WHERE (scorched3d_events.playerid=$playerid) and (TO_DAYS(NOW()) - TO_DAYS(scorched3d_events.eventtime) < 1) and scorched3d_events.prefixid=$prefixid and scorched3d_events.seriesid=$seriesid group by playerid limit 1";
$result = mysql_query($query) or die("Query failed : " . mysql_error());
?>
<table width=600 border="0" align="center">
<tr><td align=center><b><?=$playername?>'s Stats For Today</b></td></tr>
</table>
<table width=600 bordercolor=#333333 cellspacing="0" cellpadding="0" border="1" align="center">
<tr>
<td bgcolor=#111111 width=50></td>
<td bgcolor=#111111 align=center><b>Player Name</b></td>
<td bgcolor=#111111 align=center><b>Kills</b></td>
<td bgcolor=#111111 align=center><b>Rnd Wins</b></td>
<td bgcolor=#111111 align=center><b>Game Wins</b></td>
<td bgcolor=#111111 align=center><b>Resigns</b></td>
<td bgcolor=#111111 align=center><b>Suicides</b></td>
<td bgcolor=#111111 align=center><b>Teamkills</b></td>
</tr>
<?
$rownum=0;
while ($row = mysql_fetch_object($result))
{
	$tmp = preg_replace("/[\(-].*/", "", $row->osdesc);
        ++$rownum;
        echo "<tr>";
		echo "<td><center>$rownum</center></td></center>";
        echo "<td><a href=\"playerstats.php?Prefix=$prefixid&Series=$seriesid&PlayerID=$row->playerid\">$row->name</a></td>";
        echo "<td><center>$row->kills</td></center>";
        echo "<td><center>$row->wins</td></center>";
        echo "<td><center>$row->gamewins</td></center>";
        echo "<td><center>$row->resigns</td></center>";
        echo "<td><center>$row->selfkills</td></center>";
        echo "<td><center>$row->teamkills</td></center>";
		echo "<td>$tmp</td>";
        echo "</tr>";
}
?>
</table>
<?
$query = "select count(playerid) from scorched3d_stats where prefixid=$prefixid and seriesid=$seriesid;";
$result = mysql_query($query) or die("Query failed : " . mysql_error());
$row = mysql_fetch_array($result);
?>
<table width=600 border =0 align=center>
<tr><td><a href="allstats.php?Prefix=<?=$prefixid?>&Series=<?=$seriesid?>&Fields=skl:k:me:w:ow:r:sk:tk:d:tp&OrderBy=kills">
View all players (<? print $row[0]; ?>). 
</a>
</tr></td>
</table>
<br>

<?
// Query player's stats for the last week
$query = "select (scorched3d_events.playerid) as playerid, (scorched3d_players.name) as name, SUM(IF(scorched3d_events.eventtype='1',1,0)) AS kills, SUM(IF(scorched3d_events.eventtype='2',1,0)) AS teamkills, SUM(IF(scorched3d_events.eventtype='3',1,0)) AS selfkills, SUM(IF(scorched3d_events.eventtype='4',1,0)) AS resigns, SUM(IF(scorched3d_events.eventtype='5',1,0)) AS wins, SUM(IF(scorched3d_events.eventtype='6',1,0)) AS gamewins FROM scorched3d_events LEFT JOIN scorched3d_players ON (scorched3d_events.playerid=scorched3d_players.playerid) WHERE (scorched3d_events.prefixid=$prefixid) and (scorched3d_events.seriesid=$seriesid) and (scorched3d_events.playerid=$playerid) and (TO_DAYS(NOW()) - TO_DAYS(scorched3d_events.eventtime) < 7) group by playerid limit 1";
$result = mysql_query($query) or die("Query failed : " . mysql_error());
?>
<table width=600 border="0" align="center">
<tr><td align=center><b><?=$playername?>'s Stats For The Last 7 Days</b></td></tr>
</table>
<table width=600 bordercolor=#333333 cellspacing="0" cellpadding="0" border="1" align="center">
<tr>
<td bgcolor=#111111 width=50></td>
<td bgcolor=#111111 align=center><b>Player Name</b></td>
<td bgcolor=#111111 align=center><b>Kills</b></td>
<td bgcolor=#111111 align=center><b>Rnd Wins</b></td>
<td bgcolor=#111111 align=center><b>Game Wins</b></td>
<td bgcolor=#111111 align=center><b>Resigns</b></td>
<td bgcolor=#111111 align=center><b>Suicides</b></td>
<td bgcolor=#111111 align=center><b>Teamkills</b></td>
</tr>
<?
$rownum=0;
while ($row = mysql_fetch_object($result))
{
	$tmp = preg_replace("/[\(-].*/", "", $row->osdesc);
        ++$rownum;
        echo "<tr>";
		echo "<td><center>$rownum</center></td></center>";
        echo "<td><a href=\"playerstats.php?Prefix=$prefixid&Series=$seriesid&PlayerID=$row->playerid\">$row->name</a></td>";
        echo "<td><center>$row->kills</td></center>";
        echo "<td><center>$row->wins</td></center>";
        echo "<td><center>$row->gamewins</td></center>";
        echo "<td><center>$row->resigns</td></center>";
        echo "<td><center>$row->selfkills</td></center>";
        echo "<td><center>$row->teamkills</td></center>";
		echo "<td>$tmp</td>";
        echo "</tr>";
}
?>
</table>
<?
$query = "select count(playerid) from scorched3d_stats where prefixid=$prefixid and seriesid=$seriesid;";
$result = mysql_query($query) or die("Query failed : " . mysql_error());
$row = mysql_fetch_array($result);
?>
<table width=600 border =0 align=center>
<tr><td><a href="allstats.php?Prefix=<?=$prefixid?>&Series=<?=$seriesid?>&Fields=skl:k:me:w:ow:r:sk:tk:d:tp&OrderBy=kills">
View all players (<? print $row[0]; ?>). 
</a>
</tr></td>
</table>
<br>
	
<?
// List top targets
$query="select (scorched3d_events.otherplayerid) as killedid, (scorched3d_players.name) as name, (count(*)) AS tally from scorched3d_events LEFT JOIN scorched3d_players ON (scorched3d_events.otherplayerid=scorched3d_players.playerid) where scorched3d_events.playerid=$playerid and scorched3d_events.prefixid=$prefixid and scorched3d_events.seriesid=$seriesid and eventtype=1 group by scorched3d_events.otherplayerid order by tally desc limit 25";
$result = mysql_query($query) or die("Query failed : " . mysql_error(). "<BR>query=$query");
?>
<table width="600" border="0" align="center">
<tr><td align=center><b><?=$playername?>'s Top 25 Targets</b></td></tr>
</table>
<table width="600" bordercolor=#333333 cellspacing="0" cellpadding="0" border="1" align="center">
<tr>
<td bgcolor=#111111 width=50></td>
<td bgcolor=#111111><b>Target</b></td>
<td bgcolor=#111111><b>Number of times target killed</b></td>
</tr>
<?
$rownumber = 0;
while ($row = mysql_fetch_object($result))
{
	++$rownumber;
	echo "<tr>";
	echo "<td>$rownumber</td>";
	echo "<td><a href=\"playerstats.php?Prefix=$prefixid&Series=$seriesid&PlayerID=$row->killedid\">$row->name</a></td>";
	echo "<td>$row->tally</td>";
	echo "</tr>";
}
?>
</table>
<br>

<?
// Top Killers
$query=" select (scorched3d_events.playerid) as killedid, (scorched3d_players.name) as name, (count(*)) AS tally from scorched3d_events LEFT JOIN scorched3d_players ON (scorched3d_events.playerid=scorched3d_players.playerid) where otherplayerid=$playerid and prefixid=$prefixid and seriesid=$seriesid and eventtype=1 group by scorched3d_events.playerid order by tally desc limit 25";
$result = mysql_query($query) or die("Query failed : " . mysql_error(). "<BR>query=$query");
?>
<table width="600" border="0" align="center">
<tr><td align=center><b><?=$playername?>'s Top 25 Killers</b></td></tr>
</table>
<table width="600" bordercolor=#333333 cellspacing="0" cellpadding="0" border="1" align="center">
<tr>
<td bgcolor=#111111 width=50></td>
<td bgcolor=#111111><b>Killer</b></td>
<td bgcolor=#111111><b>Number of times killed</b></td>
</tr>
<?
$rownumber=0;
while ($row = mysql_fetch_object($result))
{
	++$rownumber;
        echo "<tr>";
        echo "<td>$rownumber</td>";
        echo "<td><a href=\"playerstats.php?Prefix=$prefixid&Series=$seriesid&PlayerID=$row->killedid\">$row->name</a></td>";
        echo "<td>$row->tally</td>";
	echo "</tr>";
}
?>
</table>
<br>

<?	
// Top Weapons
$query="select (scorched3d_events.weaponid) as weaponid, (count(*)) as tally, (scorched3d_weapons.armslevel) as armslevel, (scorched3d_weapons.name) as name from scorched3d_events LEFT JOIN scorched3d_weapons ON (scorched3d_events.weaponid=scorched3d_weapons.weaponid) where playerid=$playerid and scorched3d_events.prefixid=$prefixid and scorched3d_events.seriesid=$seriesid and eventtype=1 group by weaponid order by tally desc";
$result = mysql_query($query) or die("Query failed : " . mysql_error(). "<BR>query=$query");
?>
<table width="600" border="0" align="center">
<tr><td align=center><b><?=$playername?>'s Top Weapons</b></td></tr>
</table>
<table width="600" bordercolor=#333333 cellspacing="0" cellpadding="0" border="1" align="center">
<tr>
<td bgcolor=#111111 width=50></td>
<td bgcolor=#111111><b>Weapon</b></td>
<td bgcolor=#111111><b>Armslevel</b></td>
<td bgcolor=#111111><b>Kills</b></td>
</tr>
<?
$rownumber=0;
while ($row = mysql_fetch_object($result))
{
	++$rownumber;
	echo "<tr>";
	echo "<td>$rownumber<br></td>";
	echo "<td><a href=\"weaponstats.php?Prefix=$prefixid&Series=$seriesid&WeaponID=$row->weaponid\">$row->name</a></td>";
	echo "<td>$row->armslevel<br></td>";
	echo "<td>$row->tally<br></td>";
	echo "</tr>";
}
?>
</table>

<?
include('statsfooter.php');
?>
