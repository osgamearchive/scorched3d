<?
include('statsheader.php');
include('conversionfunctions.php')
?>

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

<table width="800" border="0" align="center">
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
<td><b><font size=-1><a href="allplayers.php?Prefix=<? echo $prefix; ?>">Full Rankings</a></font></b></td>
<td align=center width=20>|</td>
<td><b><font size=-1><a href="allweapons.php?Prefix=<? echo $prefix; ?>">Weapon Stats</a></font></b><br></td>
<td align=center width=20>|</td>
<td><b><font size=-1><a href="recentevents.php?Prefix=<? echo $prefix; ?>">Recent Events</a></font></b><br></td>
<td align=center width=20>|</td>
<td><b><font size=-1><a href="recentplayers.php?Prefix=<? echo $prefix; ?>">Recent Players</a></font></b><br></td>
<td align=center width=20>|</td>
<td><b><font size=-1><a href="statshistory.php?Prefix=<? echo $prefix; ?>">History</a></font></b><br></td>
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
$query = "select * from scorched3d".$prefix."_players order by kills desc limit 10";
$result = mysql_query($query) or die("Query failed : " . mysql_error());
?>

<table width="650" bordercolor="#333333" cellspacing="0" cellpadding="0" border="1" align="center">
<tr>
<td bgcolor=#111111 width=50></td>
<td bgcolor=#111111><b><a href="allplayers.php?Prefix=<? echo $prefix; ?>&OrderBy=name&Dir=asc">Player Name</a></b></td>
<td bgcolor=#111111 align=center><b><a href="allplayers.php?Prefix=<? echo $prefix; ?>&OrderBy=skill">Skill</a></b></td>
<td bgcolor=#111111 align=center><b><a href="allplayers.php?Prefix=<? echo $prefix; ?>&OrderBy=moneyearned">Money</a></b></td>
<td bgcolor=#111111 align=center><b><a href="allplayers.php?Prefix=<? echo $prefix; ?>&OrderBy=kills">Kills</a></b></td>
<td bgcolor=#111111 align=center><b><a href="allplayers.php?Prefix=<? echo $prefix; ?>&OrderBy=wins">Round Wins</a></b></td>
<td bgcolor=#111111 align=center><b><a href="allplayers.php?Prefix=<? echo $prefix; ?>&OrderBy=overallwinner">Game Wins</a></b></td>
<td bgcolor=#111111 align=center><center><b>Last Connected</b></center></td>
</tr>
<?
$rownum=0;	
while ($row = mysql_fetch_object($result))
{
	$skill = $row->skill + $row->wins * 150 + $row->overallwinner * 500;

	++$rownum;
	echo "<tr>";
	echo "<td align=center>$rownum</center></td>";
	echo "<td><a href=\"playerstats.php?Prefix=".$prefix."&PlayerID=".$row->playerid."\">$row->name</a></td>";
	echo "<td align=center>$skill</td>";
	echo "<td align=center>".prependnumber($row->moneyearned, "$", 0)."</td>";
	echo "<td align=center>$row->kills</td>";
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
You can sort all <? print $row[0]; ?> players by <a href="allplayers.php?Prefix=<? echo $prefix; ?>&PlayerID=0&OrderBy=resigns">resigns</a>, <a href="allplayers.php?Prefix=<? echo $prefix; ?>&PlayerID=0&OrderBy=selfkills">suicides</a> and <a href="allplayers.php?Prefix=<? echo $prefix; ?>&PlayerID=0&OrderBy=teamkills">teamkills</a> as well.
<br><br><br>
</tr></td>
</table>
<?
}
?>

<?
include('statsfooter.php');
?>
