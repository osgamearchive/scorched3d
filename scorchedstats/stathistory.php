<?//Gather any sort info or set defaults

$prefixid = ( isset($HTTP_GET_VARS['Prefix']) ) ? intval($HTTP_GET_VARS['Prefix']) : 0;
$seriesid = ( isset($HTTP_GET_VARS['Series']) ) ? intval($HTTP_GET_VARS['Series']) : 0;
$playerid = ( isset($HTTP_GET_VARS['PlayerID']) ) ? intval($HTTP_GET_VARS['PlayerID']) : 0;

$days = ( isset($HTTP_GET_VARS['Days']) ) ? intval($HTTP_GET_VARS['Days']) : 6;
$num_rows = ( isset($HTTP_GET_VARS['Players']) ) ? intval($HTTP_GET_VARS['Players']) : 10;

$orderby = ( isset($HTTP_GET_VARS['OrderBy']) ) ? htmlspecialchars($HTTP_GET_VARS['OrderBy']) : 'kills';

$dir = ( isset($HTTP_GET_VARS['Dir']) ) ? htmlspecialchars($HTTP_GET_VARS['Dir']) : 'desc';
if ($dir != "desc" && $dir != "asc") $dir = "desc";

$compare = ( isset($HTTP_GET_VARS['Compare']) ) ? htmlspecialchars($HTTP_GET_VARS['Compare']) : '<=';
if ($compare != '<=' && $compare != '<' && $compare != '=' && 
	$compare != '>' && $compare != '>=') $compare = '<=';
if ($compare=='<' and $days=='0') $compare='=';

include('statsheader.php');
include('sortfunction.php');  //function used to determine sort order
?>

<? include('util.php'); ?>
<br>
                                                                                            
<table width=640 border="1" align="center">
<tr><td align="center">

<!--Display the form for selecting the time period for the search-->
<form action="stathistory.php" method="get">
<font size="+2"><B>Stat History Options</B></font><BR>
Search Days 
<select name="Compare">
	<option value="<=" <? if ($compare=='<=') echo "selected='true'";?>>Less than or equal to</option>
	<option value="<"<? if ($compare=='<') echo "selected='true'";?>>Less than</option>
	<option value="="<? if ($compare=='=') echo "selected='true'";?>>Equal to</option>
	<option value=">"<? if ($compare=='>') echo "selected='true'";?>>Greater than</option>
	<option value=">="<? if ($compare=='>=') echo "selected='true'";?>>Greater than or equal to</option>-->
</select>
<select name="Days">
	<option value="0" <? if ($days=='0') echo "selected=true";?>>Today</option>
	<option value="1" <? if ($days=='1') echo "selected=true";?>>2 Days Ago(Yesterday)</option>
	<option value="2" <? if ($days=='2') echo "selected=true";?>>3 Days Ago</option>
	<option value="3" <? if ($days=='3') echo "selected=true";?>>4 Days Ago</option>
	<option value="4" <? if ($days=='4') echo "selected=true";?>>5 Days Ago</option>
	<option value="5" <? if ($days=='5') echo "selected=true";?>>6 Days Ago</option>
	<option value="6" <? if ($days=='6') echo "selected=true";?>>One Week Ago</option>
	<option value="13" <? if ($days=='13') echo "selected=true";?>>Two Weeks Ago</option>
	<option value="20" <? if ($days=='20') echo "selected=true";?>>Three Weeks Ago</option>
	<option value="27" <? if ($days=='27') echo "selected=true";?>>One Month Ago</option>
</select>
<input type="reset"><BR>
<input type="submit" value="Go!">
<input type="hidden" name="Prefix" value="<?=$prefixid?>">
<input type="hidden" name="Series" value="<?=$seriesid?>">
</form>

</td></tr>
</table>

<?
$query = "SELECT (scorched3d_events.playerid) as playerid, (scorched3d_players.name) as name, SUM(IF(scorched3d_events.eventtype='1',1,0)) AS kills, SUM(IF(scorched3d_events.eventtype='2',1,0)) AS teamkills, SUM(IF(scorched3d_events.eventtype='3',1,0)) AS selfkills, SUM(IF(scorched3d_events.eventtype='4',1,0)) AS resigns, SUM(IF(scorched3d_events.eventtype='5',1,0)) AS roundwins, SUM(IF(scorched3d_events.eventtype='6',1,0)) AS gamewins FROM scorched3d_events LEFT JOIN scorched3d_players ON (scorched3d_events.playerid=scorched3d_players.playerid) WHERE TO_DAYS(NOW()) - TO_DAYS(scorched3d_events.eventtime) AND prefixid=$prefixid AND seriesid=$seriesid $compare $days GROUP BY playerid ORDER BY $orderby $dir LIMIT $playerid, 25";
$result = mysql_query($query) or die("Query failed : " . mysql_error());
if ($num_rows==Null){
	$query = "SELECT playerid FROM scorched3d_events WHERE TO_DAYS(NOW()) - TO_DAYS(scorched3d_events.eventtime) and prefixid=$prefixid and seriesid=$seriesid $compare $days GROUP BY playerid";
	$result_info = mysql_query($query);
	$num_rows = mysql_num_rows($result_info);
}
if ($days!='0'){
	switch ($compare){
		case '<=': $message = 'For The Last '.($days+1).' Days'; break;
		case '<': $message = 'For The Last '.$days.' Days'; break;
		case '=': $message = 'For '.($days+1).' Days Ago'; break;
		case '>': $message = 'From All Stored Events Before '.($days+1).' Days Ago'; break;
		case '>=': $message = 'From All Stored Events Before '.($days).' Days Ago'; break;
	}
}
else{
	switch ($compare){
		case '<=':
		case '<':
		case '=': $message = 'For Today'; break;
		case '>': $message = 'From All Stored Events Before Today'; break;
		case '>=': $message = 'From All Stored Events'; break;
	}
}
?>
<table width=640 border="0" align="center">
<tr><td align=center><font size="+1"><b><? echo "Players ".($playerid+1)." to ".($playerid+25)." (sorted by ".$orderby.") Statistics ".$message;?></b></font></td></tr>
</table>
<table width=640 bordercolor=#333333 cellspacing="0" cellpadding="0" border="1" align="center">
<tr>
<td bgcolor=#111111 width=50></td>

<?
$compare=urlencode($compare);
echo "<td bgcolor=#111111><b><a href=stathistory.php?Prefix=".$prefixid."&Series=".$seriesid."&PlayerID=0&OrderBy=name&Dir=".sortdirection('name', $orderby, $dir)."&Compare=".$compare."&Days=".$days."&Players=".$num_rows.">Player Name</a></b></td>";
echo "<td bgcolor=#111111><b><a href=stathistory.php?Prefix=".$prefixid."&Series=".$seriesid."&PlayerID=0&OrderBy=kills&Dir=".sortdirection('kills', $orderby, $dir)."&Compare=".$compare."&Days=".$days."&Players=".$num_rows.">Kills</a></b></td>";
echo "<td bgcolor=#111111><b><a href=stathistory.php?Prefix=".$prefixid."&Series=".$seriesid."&PlayerID=0&OrderBy=roundwins&Dir=".sortdirection('roundwins', $orderby, $dir)."&Compare=".$compare."&Days=".$days."&Players=".$num_rows.">Round Wins</a></b></td>";
echo "<td bgcolor=#111111><b><a href=stathistory.php?Prefix=".$prefixid."&Series=".$seriesid."&PlayerID=0&OrderBy=gamewins&Dir=".sortdirection('gamewins', $orderby, $dir)."&Compare=".$compare."&Days=".$days."&Players=".$num_rows.">Game Wins</a></b></td>";
echo "<td bgcolor=#111111><b><a href=stathistory.php?Prefix=".$prefixid."&Series=".$seriesid."&PlayerID=0&OrderBy=resigns&Dir=".sortdirection('resigns', $orderby, $dir)."&Compare=".$compare."&Days=".$days."&Players=".$num_rows.">Resigns</a></b></td>";
echo "<td bgcolor=#111111><b><a href=stathistory.php?Prefix=".$prefixid."&Series=".$seriesid."&PlayerID=0&OrderBy=selfkills&Dir=".sortdirection('selfkills', $orderby, $dir)."&Compare=".$compare."&Days=".$days."&Players=".$num_rows.">Suicides</a></b></td>";
echo "<td bgcolor=#111111><b><a href=stathistory.php?Prefix=".$prefixid."&Series=".$seriesid."&PlayerID=0&OrderBy=teamkills&Dir=".sortdirection('teamkills', $orderby, $dir)."&Compare=".$compare."&Days=".$days."&Players=".$num_rows.">Team Kills</a></b></td>";
?>
</tr>
<?
$rownum=0;
while ($row = mysql_fetch_object($result))
{
        ++$rownum;
        echo "<tr>";
        echo "<td>".($rownum + $playerid)."</td>";
        echo "<td><a href=playerstats.php?Prefix=".$prefixid."&Series=".$seriesid."&PlayerID=$row->playerid>$row->name</a></td>";
        echo "<td>$row->kills</td>";
        echo "<td>$row->roundwins</td>";
        echo "<td>$row->gamewins</td>";
        echo "<td>$row->resigns</td>";
		echo "<td>$row->selfkills</td>";
		echo "<td>$row->teamkills</td>";
        echo "</tr>";
}
?>
</table>
<table width=640 align=center border=0>
<tr><td>
<table border=0 align=center>
<tr>
<?
$pages = $num_rows / 25;
$rows = 0;
if (gettype($pages)!="integer") $pages++;
for ($i=1; $i<=$pages; $i++)
{
	$startplayer = ($i - 1) * 25 + 1;
	$playerindex = $startplayer - 1;
	$endplayer = $startplayer + 24;
	echo "<td align=center><font size=-2><a href=stathistory.php?Prefix=".$prefixid."&Series=".$seriesid."&PlayerID=$playerindex&OrderBy=$orderby&Dir=$dir&Compare=$compare&Days=$days&Players=$num_rows>[$startplayer-$endplayer]</a></font></td>";
	$rows++;
	if ($rows > 7)
	{
		$rows = 0;
		echo "</tr><tr>\n";
	}
}
?>
</tr>
</table>
</td></tr>
</table>

<?
include('statsfooter.php');
?>
