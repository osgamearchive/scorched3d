<?//Gather any sort info or set defaults
$playerid=$_GET['PlayerID'];// or die ("No player specified");
if ($playerid==Null) $playerid=0;
$prefix=$_GET['Prefix'];
if ($prefix==Null)	$prefix="";
$orderby=$_GET['OrderBy'];
if ($orderby==Null)	$orderby="kills";
$dir=$_GET['Dir'];
if ($dir==Null)	$dir="desc";
$compare=$_GET['Compare'];
if ($compare==Null) $compare="<=";
$days=$_GET['Days'];
if ($days==Null) $days="6";
$num_rows=$_GET['Players'];
if ($compare=='<' and $days=='0') $compare='=';
include('statsheader.php');
include('sortfunction.php');  //function used to determine sort order
?>
                                                                                            
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
<input type="hidden" name="Prefix" value="<?=$prefix?>">
</form>

</td></tr>
</table>

<?
$query = "SELECT (scorched3d".$prefix."_events.playerid) as playerid, (scorched3d".$prefix."_players.name) as name, (scorched3d".$prefix."_players.lastconnected) as lastconnected, SUM(IF(scorched3d".$prefix."_events.eventtype='1',1,0)) AS kills, SUM(IF(scorched3d".$prefix."_events.eventtype='2',1,0)) AS teamkills, SUM(IF(scorched3d".$prefix."_events.eventtype='3',1,0)) AS selfkills, SUM(IF(scorched3d".$prefix."_events.eventtype='4',1,0)) AS resigns, SUM(IF(scorched3d".$prefix."_events.eventtype='5',1,0)) AS roundwins, SUM(IF(scorched3d".$prefix."_events.eventtype='6',1,0)) AS gamewins FROM scorched3d".$prefix."_events LEFT JOIN scorched3d".$prefix."_players ON (scorched3d".$prefix."_events.playerid=scorched3d".$prefix."_players.playerid) WHERE TO_DAYS(NOW()) - TO_DAYS(scorched3d".$prefix."_events.eventtime) $compare $days GROUP BY playerid ORDER BY $orderby $dir LIMIT $playerid, 25";
$result = mysql_query($query) or die("Query failed : " . mysql_error());
if ($num_rows==Null){
	$query = "SELECT playerid FROM scorched3d".$prefix."_events WHERE TO_DAYS(NOW()) - TO_DAYS(scorched3d".$prefix."_events.eventtime) $compare $days GROUP BY playerid";
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
echo "<td bgcolor=#111111><b><a href=stathistory.php?Prefix=".$prefix."&PlayerID=0&OrderBy=name&Dir=".sortdirection('name', $orderby, $dir)."&Compare=".$compare."&Days=".$days."&Players=".$num_rows.">Player Name</a></b></td>";
echo "<td bgcolor=#111111><b><a href=stathistory.php?Prefix=".$prefix."&PlayerID=0&OrderBy=kills&Dir=".sortdirection('kills', $orderby, $dir)."&Compare=".$compare."&Days=".$days."&Players=".$num_rows.">Kills</a></b></td>";
echo "<td bgcolor=#111111><b><a href=stathistory.php?Prefix=".$prefix."&PlayerID=0&OrderBy=roundwins&Dir=".sortdirection('roundwins', $orderby, $dir)."&Compare=".$compare."&Days=".$days."&Players=".$num_rows.">Round Wins</a></b></td>";
echo "<td bgcolor=#111111><b><a href=stathistory.php?Prefix=".$prefix."&PlayerID=0&OrderBy=gamewins&Dir=".sortdirection('gamewins', $orderby, $dir)."&Compare=".$compare."&Days=".$days."&Players=".$num_rows.">Game Wins</a></b></td>";
echo "<td bgcolor=#111111><b><a href=stathistory.php?Prefix=".$prefix."&PlayerID=0&OrderBy=resigns&Dir=".sortdirection('resigns', $orderby, $dir)."&Compare=".$compare."&Days=".$days."&Players=".$num_rows.">Resigns</a></b></td>";
echo "<td bgcolor=#111111><b><a href=stathistory.php?Prefix=".$prefix."&PlayerID=0&OrderBy=selfkills&Dir=".sortdirection('selfkills', $orderby, $dir)."&Compare=".$compare."&Days=".$days."&Players=".$num_rows.">Suicides</a></b></td>";
echo "<td bgcolor=#111111><b><a href=stathistory.php?Prefix=".$prefix."&PlayerID=0&OrderBy=teamkills&Dir=".sortdirection('teamkills', $orderby, $dir)."&Compare=".$compare."&Days=".$days."&Players=".$num_rows.">Team Kills</a></b></td>";
//echo "<td width=160 bgcolor=#111111><b><a href=stathistory.php?Prefix=".$prefix."&PlayerID=0&OrderBy=lastconnected&Dir=".sortdirection('lastconnected', $orderby, $dir)."&Compare=".$compare."&Days=".$days."&Players=".$num_rows.">Last Connect</b></td>";
?>
</tr>
<?
$rownum=0;
while ($row = mysql_fetch_object($result))
{
        ++$rownum;
        echo "<tr>";
        echo "<td>".($rownum + $playerid)."</td>";
        echo "<td><a href=playerstats.php?Prefix=".$prefix."&PlayerID=$row->playerid>$row->name</a></td>";
        echo "<td>$row->kills</td>";
        echo "<td>$row->roundwins</td>";
        echo "<td>$row->gamewins</td>";
        echo "<td>$row->resigns</td>";
		echo "<td>$row->selfkills</td>";
		echo "<td>$row->teamkills</td>";
        //echo "<td>$row->lastconnected</td>";
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
	echo "<td align=center><font size=-2><a href=stathistory.php?Prefix=".$prefix."&PlayerID=$playerindex&OrderBy=$orderby&Dir=$dir&Compare=$compare&Days=$days&Players=$num_rows>[$startplayer-$endplayer]</a></font></td>";
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
