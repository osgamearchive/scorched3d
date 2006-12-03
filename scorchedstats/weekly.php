<?
include('statsheader.php');
?>

<table width="790" border="0" align="center">
<tr align=center>
<td>
<font size=+2><b><u>Weekly Stats (For the last 7 days)</font><br>
</td>
</tr>
</table>

<?

show_stat("Most Deadly", 1, "Kills");

show_stat("Most Suicidal", 3, "Suicides");

show_stat("Most Cowardly", 4, "Resigns");

show_stat("Most Victorious", 6, "Wins");

show_stat("Most Friendly", 9, "Joined");

include('statsfooter.php');
?>

<?

function show_stat($title, $eventtype, $eventname)
{
	$sql = 

		"select avatarid, name, count(name) as scount ".
		"from scorched3d_events left join scorched3d_players on ".
		"scorched3d_players.playerid = scorched3d_events.playerid ".
		"where ".
		"eventtype = " .$eventtype. " ".
		"and ".
		"TO_DAYS(NOW()) - TO_DAYS(scorched3d_events.eventtime) <= 7 ".
		"group by scorched3d_events.playerid order by scount desc limit 0, 10";
	if( !($result = mysql_query($sql)) )
	{
		message_die(GENERAL_ERROR, "Could run query $eventtype");
	}

?>

<br>
<table width="650" border="0" align="center">
<tr>
<td align=Center>
<b><font size="+1"><?=$title?></font></b>
</td>
</tr>
</td>

<table width="650" bordercolor="#333333" cellspacing="0" cellpadding="0" border="1" align="center">
<tr>
<td bgcolor=#111111 width=50></td>
<td bgcolor=#111111>Player Name</td>
<td bgcolor=#111111 width=150><?=$eventname?></td>
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
	echo $row->name;
	echo "</td>";
	echo "</tr></table>";
	echo "</td>";
	echo "<td align=center>$row->scount</td>";
	echo "</tr>";
	}

?>

</table>

<?
}
?>

