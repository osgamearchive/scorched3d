<?
include('statsheader.php');
include('conversionfunctions.php')
?>

<p align=center>
<b>
<? echo $other_title; ?><br>
<font size=+2>
<? echo $main_title; ?>
</font>
</b>
</p>

<?
// Servers
$query=" select * from scorched3d_main";
$outerresult = mysql_query($query) or die("Query failed : " . mysql_error());
?>

<?
while ($outerrow = mysql_fetch_object($outerresult))
{
$prefix = $outerrow->prefix;
?>

<?
$countresult = mysql_query("select count(playerid) from scorched3d".$prefix."_players") or die("Query failed : " . mysql_error());
$countrow = mysql_fetch_array($countresult);
$count = $countrow[0];
?>

<table width="790" border="0" align="center">
<tr>
<td align=Center><b><font size="+1"><? echo $outerrow->name; ?></font></b><br>
<font size=-1>Players : <? echo $count; ?>, Rounds : <? echo $outerrow->rounds; ?>, 
Games : <? echo $outerrow->games; ?></font><br>
</td>
</tr>

<tr>
<td align=center>
<table align=center border=0>
<tr>
<td><b><font size=-1><a href="allstats.php?Prefix=<? echo $prefix; ?>&Fields=skl:k:me:w:ow:r:sk:tk:d:tp&OrderBy=kills">Rankings</a></font></b></td>
<td align=center width=20>|</td>
<td><b><font size=-1><a href="allstats.php?Prefix=<? echo $prefix; ?>&Fields=skl:k:ow:kr:wr:mpk:mph:tp&OrderBy=skill">Extended Rankings</a></font></b></td>
<td align=center width=20>|</td>
<td><b><font size=-1><a href="allweapons.php?Prefix=<? echo $prefix; ?>">Weapon Stats</a></font></b><br></td>
<td align=center width=20>|</td>
<td><b><font size=-1><a href="recentevents.php?Prefix=<? echo $prefix; ?>">Recent Events</a></font></b><br></td>
<td align=center width=20>|</td>
<td><b><font size=-1><a href="recentplayers.php?Prefix=<? echo $prefix; ?>">Recent Players</a></font></b><br></td>
<td align=center width=20>|</td>
<td><b><font size=-1><a href="stathistory.php?Prefix=<? echo $prefix; ?>">History</a></font></b><br></td>
<td align=center width=20>|</td>
<td><b><font size=-1><a href="playeros.php?Prefix=<? echo $prefix; ?>">Server Stats</a></font></b><br></td>
</tr>
</table>
</td>
</tr>

<tr>
<td>
<?
// Top Players Query
$query = "select *, round((skill + (overallwinner*5) + (COALESCE(round((wins/gamesplayed)*(skill-1000), 3), 0)) + ((skill-1000) * COALESCE(round(((kills-(teamkills+selfkills))/shots), 3), 0))),0) as skill from scorched3d".$prefix."_players order by kills desc limit 10";
$result = mysql_query($query) or die("Query failed : " . mysql_error());
?>

<table width="650" bordercolor="#333333" cellspacing="0" cellpadding="0" border="1" align="center">
<tr>
<td bgcolor=#111111 width=50></td>
<td bgcolor=#111111><b><a href="allstats.php?Prefix=<? echo $prefix; ?>&Fields=skl:k:me:w:ow:r:sk:tk:d:tp&OrderBy=name&Dir=asc">Player Name</a></b></td>
<td bgcolor=#111111 align=center><b><a href="allstats.php?Prefix=<? echo $prefix; ?>&Fields=skl:k:me:w:ow:r:sk:tk:d:tp&OrderBy=skill">Skill</a></b></td>
<td bgcolor=#111111 align=center><b><a href="allstats.php?Prefix=<? echo $prefix; ?>&Fields=skl:k:me:w:ow:r:sk:tk:d:tp&OrderBy=kills">Kills</a></b></td>
<td bgcolor=#111111 align=center><b><a href="allstats.php?Prefix=<? echo $prefix; ?>&Fields=skl:k:me:w:ow:r:sk:tk:d:tp&OrderBy=moneyearned">Money</a></b></td>
<td bgcolor=#111111 align=center><b><a href="allstats.php?Prefix=<? echo $prefix; ?>&Fields=skl:k:me:w:ow:r:sk:tk:d:tp&OrderBy=wins">Round Wins</a></b></td>
<td bgcolor=#111111 align=center><b><a href="allstats.php?Prefix=<? echo $prefix; ?>&Fields=skl:k:me:w:ow:r:sk:tk:d:tp&OrderBy=gamewins">Game Wins</a></b></td>
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
	echo "<a href=\"playerstats.php?Prefix=".$prefix."&PlayerID=".$row->playerid."\">";
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
You can sort all <? print $row[0]; ?> players by <a href="allstats.php?Prefix=<? echo $prefix; ?>&Fields=skl:k:me:w:ow:r:sk:tk:d:tp&OrderBy=resigns">resigns</a>, <a href="allstats.php?Prefix=<? echo $prefix; ?>&Fields=skl:k:me:w:ow:r:sk:tk:d:tp&OrderBy=selfkills">suicides</a> and <a href="allstats.php?Prefix=<? echo $prefix; ?>&Fields=skl:k:me:w:ow:r:sk:tk:d:tp&OrderBy=teamkills">teamkills</a> as well.
<?
include('search.php');
?>
<br><br><br>
</tr></td>
</table>
<?
}
?>

<?
include('statsfooter.php');
?>
