<?
include('statsheader.php');
?>

<p align="center"><b><? echo $main_title; ?></b>
<BR><A href="playeros.php">Player Operating Systems</A>
<BR><A href="recentplayers.php">Recent Players List</A>
<BR><A href="recentevents.php">Recent Events</A></p>

<?
// Top Players Query
$query = " select * from scorched3d_players order by kills desc limit 10";
$result = mysql_query($query) or die("Query failed : " . mysql_error());
?>
<table width="600" border="0" align="center">
<tr><td align=center><b>Top 10 Killers For All Time</b></td></tr>
</table>
<table width="600" bordercolor="#333333" cellspacing="0" cellpadding="0" border="1" align="center">
<tr>
<td bgcolor=#111111 width=50></td>
<td bgcolor=#111111><center><b>Player Name</b></center></td>
<td width=60 bgcolor=#111111><center><b>Kills</b></center></td>
<td width=70 bgcolor=#111111><center><b>Kill Ratio</b></center></td>
<td width=60 bgcolor=#111111><center><b>Round Wins</b></center></td>
<td width=60 bgcolor=#111111><center><b>Game Wins</b></center></td>
<td bgcolor=#111111><center><b>Last Connected</b></center></td>
</tr>
<?
$rownum=0;	
while ($row = mysql_fetch_object($result))
{
	$tmp = preg_replace("/[\(-].*/", "", $row->osdesc);
	if ($row->shots != 0)
		$killratio = round(($row->kills-($row->teamkills+$row->selfkills))/$row->shots, 3)*100;
	else
		$killratio = 0.0;
	++$rownum;
	echo "<tr>";
	echo "<td><center>$rownum</center></td></center>";
	echo "<td><a href=playerstats.php?PlayerID=$row->playerid>$row->name</a></td>";
	echo "<td><center>$row->kills</td></center>";
	echo "<td><center>$killratio%</td></center>";
	echo "<td><center>$row->wins</td></center>";
	echo "<td><center>$row->overallwinner</td></center>";
	echo "<td><center>$row->lastconnected</td></center>";
	echo "</tr>";
}
?>
</table>
<?
$query = "select count(playerid) from scorched3d_players;";
$result = mysql_query($query) or die("Query failed : " . mysql_error());
$row = mysql_fetch_array($result);
?>
<table width=600 border =0 align=center>
<tr><td><a href="allplayers.php?PlayerID=0">
<center>View all players (<? print $row[0]; ?>).</center>
</a>
</tr></td>
</table>
<br>

<?
// Most Accurate Players Query
$query = " select playerid, kills, lastconnected, shots, name, teamkills, selfkills, deaths, round(((kills-(teamkills+selfkills))/shots)*100, 1) as killratio, round((kills-teamkills)/deaths, 2) as killsperdeath from scorched3d_players WHERE (kills >= 15) ORDER BY killratio desc limit 10";
$result = mysql_query($query) or die("Query failed : " . mysql_error());
?>
<table width="600" border="0" align="center">
<tr><td align=center><b>All Time Deadliest Players</b></td></tr>
</table>
<table width="600" bordercolor="#333333" cellspacing="0" cellpadding="0" border="1" align="center">
<tr>
<td bgcolor=#111111 width=50></td>
<td bgcolor=#111111><center><b>Player Name</b></center></td>
<td width=60 bgcolor=#111111><center><b>Kills</b></center></td>
<td width=70 bgcolor=#111111><center><b>Kill Ratio</b></center></td>
<td width=60 bgcolor=#111111><center><b>Kills / Death</b></center></td>
<td bgcolor=#111111><center><b>Last Connect</b></center></td>
</tr>
<?
$rownum=0;	
while ($row = mysql_fetch_object($result))
{
	if ($row->shots != 0)
		$killratio = round((($row->kills-($row->teamkills+$row->selfkills))/$row->shots)*100, 1);
	else
		$killratio = 0.0;
	if ($row->deaths != 0)
		$killsperdeath = round($row->kills/$row->deaths,2);
	else
		$killsperdeath = "No Deaths";
	++$rownum;
	echo "<tr>";
	echo "<td><center>$rownum</center></td></center>";
	echo "<td><a href=playerstats.php?PlayerID=$row->playerid>$row->name</a></td>";
	echo "<td><center>$row->kills</td></center>";
	echo "<td><center>$killratio%</td></center>";
	echo "<td><center>$killsperdeath</td></center>";
	echo "<td><center>$row->lastconnected</td></center>";
	echo "</tr>";
}
?>
</table><BR>

<?
// Top Players Today Query
$query = "select (scorched3d_events.playerid) as playerid, (scorched3d_players.name) as name, (scorched3d_players.lastconnected) as lastcon, SUM(IF(scorched3d_events.eventtype='1',1,0)) AS killcount, SUM(IF(scorched3d_events.eventtype='5',1,0)) AS wincount, SUM(IF(scorched3d_events.eventtype='6',1,0)) AS gamewins FROM scorched3d_events LEFT JOIN scorched3d_players ON (scorched3d_events.playerid=scorched3d_players.playerid) WHERE TO_DAYS(NOW()) - TO_DAYS(scorched3d_events.eventtime) < 1 group by playerid order by killcount desc limit 10";
$result = mysql_query($query) or die("Query failed : " . mysql_error());
?>
<table width=600 border="0" align="center">
<tr><td align=center><b>Today's Top 10 Killers</b></td></tr>
</table>
<table width=600 bordercolor=#333333 cellspacing="0" cellpadding="0" border="1" align="center">
<tr>
<td bgcolor=#111111 width=50></td>
<td bgcolor=#111111><center><b>Player Name</b></center></td>
<td bgcolor=#111111><center><b>Kills</b></center></td>
<td bgcolor=#111111><center><b>Rnd Wins</b></center></td>
<td bgcolor=#111111><center><b>Game Wins</b></center></td>
<td bgcolor=#111111><center><b>Last Connect</b></center></td>
</tr>
<?
$rownum=0;
while ($row = mysql_fetch_object($result))
{
        ++$rownum;
        echo "<tr>";
		echo "<td><center>$rownum</center></td></center>";
        echo "<td><a href=\"playerstats.php?PlayerID=$row->playerid\">$row->name</a></td>";
        echo "<td><center>$row->killcount</td></center>";
        echo "<td><center>$row->wincount</td></center>";
        echo "<td><center>$row->gamewins</td></center>";
        echo "<td><center>$row->lastcon</td></center>";
        echo "</tr>";
}
?>
</table><br>

<?
// Top Last Week Query
$query = "select (scorched3d_events.playerid) as playerid, (scorched3d_players.name) as name, (scorched3d_players.lastconnected) as lastcon, SUM(IF(scorched3d_events.eventtype='1',1,0)) AS killcount, SUM(IF(scorched3d_events.eventtype='5',1,0)) AS wincount, SUM(IF(scorched3d_events.eventtype='6',1,0)) AS gamewins FROM scorched3d_events LEFT JOIN scorched3d_players ON (scorched3d_events.playerid=scorched3d_players.playerid) WHERE TO_DAYS(NOW()) - TO_DAYS(scorched3d_events.eventtime) < 7 group by playerid order by killcount desc limit 10";
$result = mysql_query($query) or die("Query failed : " . mysql_error());
?>
<table width=600 border="0" align="center">
<tr><td align=center><b>This Week's Top 10 Killers</b></td></tr>
</table>
<table width=600 bordercolor=#333333 cellspacing="0" cellpadding="0" border="1" align="center">
<tr>
<td bgcolor=#111111 width=50></td>
<td bgcolor=#111111><center><b>Player Name</b></center></td>
<td bgcolor=#111111><center><b>Kills</b></center></td>
<td bgcolor=#111111><center><b>Rnd Wins</b></center></td>
<td bgcolor=#111111><center><b>Game Wins</b></center></td>
<td bgcolor=#111111><center><b>Last Connect</b></center></td>
</tr>
<?
$rownum=0;
while ($row = mysql_fetch_object($result))
{
        ++$rownum;
        echo "<tr>";
		echo "<td><center>$rownum</center></td>";
        echo "<td><a href=\"playerstats.php?PlayerID=$row->playerid\">$row->name</a></td>";
        echo "<td><center>$row->killcount</center></td>";
        echo "<td><center>$row->wincount</center></td>";
        echo "<td><center>$row->gamewins</center></td>";
        echo "<td><center>$row->lastcon</center></td>";
        echo "</tr>";
}
?>
</table><br>

<?
// Top Round Winners
$query="SELECT playerid, name, wins FROM scorched3d_players GROUP BY playerid order by wins desc limit 10";
$result = mysql_query($query) or die("Query failed : " . mysql_error());
?>
<table width="500" border="0" align="center">
<tr><td align=center><b>Top Round Winners</b></td></tr>
</table>
<table width="500" bordercolor=#333333 cellspacing="0" cellpadding="0" border="1" align="Center">
<tr>
<td bgcolor=#111111 width=50></td>
<td bgcolor=#111111><center><b>Name</b></center></td>
<td bgcolor=#111111><center><b>Round Wins</b></center></td>
</tr>
<?
$rownum=0;	
while ($row = mysql_fetch_object($result))
{
	++$rownum;	
	echo "<tr>";
	echo "<td><center>$rownum</center></td>";
	echo "<td><a href=\"playerstats.php?PlayerID=$row->playerid\">$row->name</a></td>";
	echo "<td><center>$row->wins</center></td>";
	echo "</tr>";
}
?>
</table><br>
<?

// Top Game Winners
$query="SELECT playerid, name, overallwinner FROM scorched3d_players GROUP BY playerid order by overallwinner desc limit 10";
$result = mysql_query($query) or die("Query failed : " . mysql_error());
?>
<table width="500" border="0" align="center">
<tr><td align=center><b>Top Game Winners</b></td></tr>
</table>
<table width="500" bordercolor=#333333 cellspacing="0" cellpadding="0" border="1" align="Center">
<tr>
<td bgcolor=#111111 width=50></td>
<td bgcolor=#111111><center><b>Name</b></center></td>
<td bgcolor=#111111><center><b>Game Wins</b></center></td>
</tr>
<?
$rownum=0;	
while ($row = mysql_fetch_object($result))
{
	++$rownum;	
	echo "<tr>";
	echo "<td><center>$rownum</center></td>";
	echo "<td><a href=\"playerstats.php?PlayerID=$row->playerid\">$row->name</a></td>";
	echo "<td><center>$row->overallwinner</center></td>";
	echo "</tr>";
}
?>
</table><br>

<?
// Most Cowardly Players
$query="SELECT playerid, name, resigns FROM scorched3d_players GROUP BY playerid order by resigns desc limit 10";
$result = mysql_query($query) or die("Query failed : " . mysql_error());
?>
<table width="500" border="0" align="center">
<tr><td align=center><b>Most Cowardly Players</b></td></tr>
</table>
<table width="500" bordercolor=#333333 cellspacing="0" cellpadding="0" border="1" align="Center">
<tr>
<td bgcolor=#111111 width=50></td>
<td bgcolor=#111111><center><b>Name</b></center></td>
<td bgcolor=#111111><center><b>Resigns</b></center></td>
</tr>
<?
$rownum=0;	
while ($row = mysql_fetch_object($result))
{
	++$rownum;	
	echo "<tr>";
	echo "<td><center>$rownum</center></td>";
	echo "<td><a href=\"playerstats.php?PlayerID=$row->playerid\">$row->name</a></td>";
	echo "<td><center>$row->resigns</center></td>";
	echo "</tr>";
}
?>
</table><br>

<?
// Most Suicidal Players
$query="SELECT playerid, name, selfkills FROM scorched3d_players GROUP BY playerid order by selfkills desc limit 10";
$result = mysql_query($query) or die("Query failed : " . mysql_error());
?>
<table width="500" border="0" align="center">
<tr><td align=center><b>Most Suicidal Players</b></td></tr>
</table>
<table width="500" bordercolor=#333333 cellspacing="0" cellpadding="0" border="1" align="Center">
<tr>
<td bgcolor=#111111 width=50></td>
<td bgcolor=#111111><center><b>Name</b></center></td>
<td bgcolor=#111111><center><b>Suicides</b></center></td>
</tr>
<?
$rownum=0;	
while ($row = mysql_fetch_object($result))
{
	++$rownum;	
	echo "<tr>";
	echo "<td><center>$rownum</center></td>";
	echo "<td><a href=\"playerstats.php?PlayerID=$row->playerid\">$row->name</a></td>";
	echo "<td><center>$row->selfkills</center></td>";
	echo "</tr>";
}
?>
</table><br>

<?
// Top weapons query
$query="select weaponid, name, kills from scorched3d_weapons where kills > 0 group by weaponid order by kills desc";
$result = mysql_query($query) or die("Query failed : " . mysql_error());
?>
<table width="500" border="0" align="center">
<tr><td align=center><b>All Time Top Weapons</b></td></tr>
</table>
<table width="500" bordercolor=#333333 cellspacing="0" cellpadding="0" border="1" align="Center">
<tr>
<td bgcolor=#111111 width=50></td>
<td bgcolor=#111111><center><b>Weapon</b></center></td>
<td bgcolor=#111111><center><b>Kills</b></center></td>
</tr>
<?
$rownum=0;	
while ($row = mysql_fetch_object($result))
{
	++$rownum;	
	echo "<tr>";
	echo "<td><center>$rownum</center></td>";
	echo "<td><a href=\"weaponstats.php?WeaponID=$row->weaponid\">$row->name</a></td>";
	echo "<td><center>$row->kills</center></td>";
	echo "</tr>";
}
?>
</table><br>

<?
// Servers
$query=" select * from scorched3d_main";
$result = mysql_query($query) or die("Query failed : " . mysql_error());
?>
<table width="500" border="0" align="center">
<tr><td align=center><b>Participating Servers</b></td></tr>
</table>
<table width="500" bordercolor=#333333 cellspacing="0" cellpadding="0" border="1" align="Center">
<tr>
<td bgcolor=#111111><b>Server</b></td>
<td bgcolor=#111111><b>Total Rounds</b></td>
<td bgcolor=#111111><b>Total Games</b></td>
</tr>
<?
$rownum=0;
while ($row = mysql_fetch_object($result))
{
        ++$rownum;
        echo "<tr>";
        echo "<td>$row->name</td>";
        echo "<td>$row->rounds</td>";
        echo "<td>$row->games</td>";
        echo "</tr>";
}
?>
</table><br>

<?
include('statsfooter.php');
?>
