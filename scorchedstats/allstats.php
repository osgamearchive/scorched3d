<?
include('statsheader.php');
include('sortfunction.php');
include('conversionfunctions.php');

$prefix=$_GET['Prefix'];
if ($prefix==Null)	$prefix="";

// Looks to see if $valuetocheck exists in the array of currently displayed fields
function fieldcheck($datastring, $valuetocheck){
	$valuearray = explode(":",$datastring);
	if (in_array($valuetocheck, $valuearray)==True){
	 	return " selected=true>";
	}
	else {
		return ">";
	}
}

// Used for selecting current options in select boxes
function selectcheck($selectedvalue, $valuetocheck){
	if ($selectedvalue==$valuetocheck) return " selected=true>";
	else return ">";
}

// In an effort to reduce url lengths, the field names are abbreviated when passed through the url
// This function returns the full field name from the abbreviated one
function expandfieldname($fieldname){
	switch ($fieldname){
		case 'skl': return 'skill'; break;
		case 'k': return 'kills'; break;
		case 'kr': return 'killratio'; break;
		case 'kpr': return 'killsperround'; break;
		case 'kph': return 'killsperhour'; break;
		case 'kpd': return 'killsperdeath'; break;
		case 'd': return 'deaths'; break;
		case 's': return 'shots'; break;
		case 'ow': return 'overallwinner'; break;
		case 'me': return 'moneyearned'; break;
		case 'mpr': return 'moneyperround'; break;
		case 'mph': return 'moneyperhour'; break;
		case 'mpk': return 'moneyperkill'; break;
		case 'rp': return 'roundsplayed'; break;
		case 'gp': return 'gamesplayed'; break;
		case 'w': return 'wins'; break;
		case 'wr': return 'winratio'; break;
		case 'rpr': return 'roundsperresign'; break;
		case 'r': return 'resigns'; break;
		case 'sk': return 'selfkills'; break;
		case 'tk': return 'teamkills'; break;
		case 'tp': return 'timeplayed'; break;
		case 'tpc': return 'timeperconnect'; break;
		case 'c': return 'connections'; break;
		case 'lc': return 'lastconnected'; break;
		case 'pid': return 'playerid'; break;
		case 'osd': return 'osdesc'; break;
		default: return $fieldname;
	}
}

// Takes a field name and returns a queriable string for use in a SELECT statement
function queryformat($fieldname){
	switch ($fieldname){
		case 'playerid':
		case 'osdesc':
		case 'lastconnected':
		case 'kills':
		case 'deaths':
		case 'selfkills':
		case 'teamkills':
		case 'connects':
		case 'shots':
		case 'wins':
		case 'overallwinner':
		case 'resigns':
		case 'gamesplayed':
		case 'timeplayed':
		case 'roundsplayed':
		case 'moneyearned': return $fieldname; break;
		case 'skill': return "round((skill + (overallwinner*5) + (COALESCE(round((wins/gamesplayed)*(skill-1000), 3), 0)) + ((skill-1000) * COALESCE(round(((kills-(teamkills+selfkills))/shots), 3), 0))),0) as skill"; break;
		case 'killratio': return "COALESCE(round(((kills-(teamkills+selfkills))/shots)*100, 2), 0.0) as killratio"; break;
		case 'killsperround': return "COALESCE(round(((kills-(teamkills+selfkills))/gamesplayed), 2), 0.0) as killsperround"; break;
		case 'killsperhour': return "COALESCE(round(((kills-(teamkills+selfkills))/(timeplayed/3600)), 2), 0.0) as killsperhour"; break;
		case 'killsperdeath': return "COALESCE(round(((kills-(teamkills+selfkills))/deaths), 2), 0.0) as killsperdeath"; break;
		case 'moneyperround': return "COALESCE(round(moneyearned/gamesplayed, 2), 0.0) as moneyperround"; break;
		case 'moneyperhour': return "COALESCE(round(moneyearned/(timeplayed/3600), 2), 0.0) as moneyperhour"; break;
		case 'moneyperkill': return "COALESCE(round(moneyearned/kills, 2), 0.0) as moneyperkill"; break;
		case 'winratio': return "COALESCE(round((wins/gamesplayed)*100, 2), 0.0) as winratio"; break;
		case 'roundsperresign': return "COALESCE(round(gamesplayed/resigns, 2), 0.0) as roundsperresign"; break;
		case 'timeperconnect': return "COALESCE(round(timeplayed/connects, 0), 0.0) as timeperconnect"; break;
		default: return '';
	}
}

// Takes a field name and returns a proper column name for displaying purposes
function columnformat($fieldname){
	switch ($fieldname) {
		case 'name': return "Player Name"; break;
		case 'playerid': return "Player ID"; break;
		case 'lastconnected': return "Last Connected"; break;
		case 'kills': return "Kills"; break;
		case 'killratio': return "Kill Ratio"; break;
		case 'killsperround': return "Kills Per Round"; break;
		case 'killsperhour': return "Kills Per Hour"; break;
		case 'killsperdeath': return "Kill Per Death"; break;
		case 'deaths': return "Deaths"; break;
		case 'selfkills': return "SKs"; break;
		case 'teamkills': return "TKs"; break;
		case 'connects': return "Connects"; break;
		case 'shots': return "Shots"; break;
		case 'wins': return "Round Wins"; break;
		case 'overallwinner': return "Game Wins"; break;
		case 'resigns': return "Resigns"; break;
		case 'gamesplayed': return "Total Rounds"; break;
		case 'skill': return "Skill"; break;
		case 'roundsperresign': return "Rounds Per Resign"; break;
		case 'roundsplayed': return "Total Turns"; break;
		case 'moneyearned': return "Money Earned"; break;
		case 'moneyperround': return "Money Per Round"; break;
		case 'moneyperhour': return "Money Per Hour"; break;
		case 'moneyperkill': return "Money Per Kill"; break;
		case 'winratio': return "Win Ratio"; break;
		case 'timeperconnect': return "Time Per Connect"; break;
		case 'timeplayed': return "Time Played"; break;
		case 'osdesc': return "OS"; break;
		default: return '';
	}
}

// Takes a field name and a value and returns an appropriately formatted string for displaying the data
function dataformat($fieldname, $value){
	switch ($fieldname) {
		case 'playerid':
		case 'lastconnected':
		case 'kills':
		case 'deaths':
		case 'selfkills':
		case 'teamkills':
		case 'connects':
		case 'shots':
		case 'wins':
		case 'overallwinner':
		case 'resigns':
		case 'gamesplayed':
		case 'skill':
		case 'killsperround':
		case 'killsperhour':
		case 'killsperdeath':
		case 'roundsperresign':
		case 'roundsplayed': return "<td align=center>".$value."</td>"; break;
		case 'moneyearned':
		case 'moneyperround':
		case 'moneyperhour':
		case 'moneyperkill': return "<td align=right>".prependnumber($value, "$", 2)."</td>";
		case 'killratio':
		case 'winratio': return "<td align=center>".prependnumber($value, "%", 1)."</td>";
		case 'timeperconnect':
		case 'timeplayed': return "<td align=right>".secondstotext($value)."</td>";
		case 'osdesc': return "<td>$value</td>"; break;
		default: return '';
	}
}

// Get playerid, sort direction and field to sort by from url
$playerid=$_GET['PlayerID'];
if ($playerid==Null or !is_numeric($playerid)) $playerid=0;

$dir=$_GET['Dir'];
if ($dir==Null)	$dir="desc";
elseif ($dir!="desc" and $dir!="asc") $dir="desc";

$orderby=$_GET['OrderBy'];
if ($orderby==Null) {
	$orderby="name";
	$dir='asc';
}

// Get selected fields to display from the url
$fielddata=$_GET['Fields'];
if ($fielddata==Null) {
	$fieldarray[0]="kills";
	$fields="k";
}
elseif (is_array($fielddata)) {
	$fieldarray=$fielddata;
	$fields=implode(":",$fielddata);
}
else {
	$fieldarray=explode(":",$fielddata);
	$fields=$fielddata;
}
$fielddata=Null;
for ($i=0; $i<count($fieldarray); $i++) {	//Expand field names in the field name array
	$fieldname=$fieldarray[$i];
	$fieldarray[$i]=expandfieldname($fieldname);
}

// Gather filtering options from the url
$filterby=$_GET['FBy'];
if ($filterby != Null and $filterby != 'none') {
	$filtercompare=$_GET['FComp'];
	$filtervalue=$_GET['FVal'];
	if ($filtercompare != Null and $filtervalue != Null) {
		$filterurl=$filterby."&FComp=".urlencode($filtercompare)."&FVal=".$filtervalue;
	}
	else $filterurl='none';
}
else {
	$filterby='none';
	$filterurl='none';
}

// Get number of results per page from the url
$num_rows=$_GET['Players'];
$limit=$_GET['Limit'];
if ($limit==Null or !is_numeric($limit)) $limit=25;
elseif ($limit<10) $limit=10;
elseif ($limit>100) $limit=100;

$urlinfo="allstats.php?Prefix=".$prefix."&Fields=".$fields."&Limit=".$limit."&FBy=".$filterurl;
?>

<!-- Begin the visible section of the page -->
<table border=1 align=center>
<tr><td align=center>
<font size="+2"<B>Advanced Stat Options</B></font>
</td></tr>

<tr><td>
	<!-- Form used to select fields, filters and results per page-->
	<form action="allstats.php" method="get">
	<table align=center>
	<tr>
		<td align=center><b>Stats To Display</b></td>
		<td align=center><b>Filtering</b></td>
		<td align=center><b>Results Per Page</b></td>
	<tr>
		<td align=center valign=middle>
			<select size="4" name="Fields[]" multiple onchange="limitSelection(cols, 8)">
				<option id='cols' value='skl'<?=fieldcheck($fields, 'skl')?>Skill</option>
				<option id='cols' value='k'<?=fieldcheck($fields, 'k')?>Kills</option>
				<option id='cols' value='kr'<?=fieldcheck($fields, 'kr')?>Kill Ratio</option>
				<option id='cols' value='kpr'<?=fieldcheck($fields, 'kpr')?>Kills/Round</option>
				<option id='cols' value='kph'<?=fieldcheck($fields, 'kph')?>Kills/Hour</option>
				<option id='cols' value='kpd'<?=fieldcheck($fields, 'kpd')?>Kills/Death</option>
				<option id='cols' value='d'<?=fieldcheck($fields, 'd')?>Deaths</option>
				<option id='cols' value='s'<?=fieldcheck($fields, 's')?>Shots</option>
				<option id='cols' value='ow'<?=fieldcheck($fields, 'ow')?>Game Wins</option>
				<option id='cols' value='me'<?=fieldcheck($fields, 'me')?>Money Earned</option>
				<option id='cols' value='mpr'<?=fieldcheck($fields, 'mpr')?>Money/Round</option>
				<option id='cols' value='mph'<?=fieldcheck($fields, 'mph')?>Money/Hour</option>
				<option id='cols' value='mpk'<?=fieldcheck($fields, 'mpk')?>Money/Kill</option>
				<option id='cols' value='rp'<?=fieldcheck($fields, 'rp')?>Turns Played</option>
				<option id='cols' value='gp'<?=fieldcheck($fields, 'gp')?>Rounds Played</option>
				<option id='cols' value='w'<?=fieldcheck($fields, 'w')?>Round Wins</option>
				<option id='cols' value='wr'<?=fieldcheck($fields, 'wr')?>Round Win Ratio</option>
				<option id='cols' value='rpr'<?=fieldcheck($fields, 'rpr')?>Rounds/Resign</option>
				<option id='cols' value='r'<?=fieldcheck($fields, 'r')?>Resigns</option>
				<option id='cols' value='sk'<?=fieldcheck($fields, 'sk')?>Suicides</option>
				<option id='cols' value='tk'<?=fieldcheck($fields, 'tk')?>Team Kills</option>
				<option id='cols' value='tp'<?=fieldcheck($fields, 'tp')?>Time Played</option>
				<option id='cols' value='tpc'<?=fieldcheck($fields, 'tpc')?>Time/Connect</option>
				<option id='cols' value='c'<?=fieldcheck($fields, 'c')?>Connections</option>
				<option id='cols' value='lc'<?=fieldcheck($fields, 'lc')?>Last Connection</option>
				<option id='cols' value='pid'<?=fieldcheck($fields, 'pid')?>Player ID</option>
				<option id='cols' value='osd'<?=fieldcheck($fields, 'osd')?>OS Description</option>
			</select>
		</td>
		<td align=center valign=middle>
			<select name="FBy">
				<option value='none'<?=selectcheck($filterby,'none');?>No Filter</option>
				<option value='playerid'<?=selectcheck($filterby,'playerid');?>Player ID</option>
				<!--<option value='skill'<?=selectcheck($filterby,'skill');?>Skill</option>-->
				<option value='kills'<?=selectcheck($filterby,'kills');?>Kills</option>
				<option value='deaths'<?=selectcheck($filterby,'deaths');?>Deaths</option>
				<option value='shots'<?=selectcheck($filterby,'shots');?>Shots</option>
				<option value='wins'<?=selectcheck($filterby,'wins');?>Round Wins</option>
				<option value='resigns'<?=selectcheck($filterby,'resigns');?>Resigns</option>
				<option value='selfkills'<?=selectcheck($filterby,'selfkills');?>Suicides</option>
				<option value='teamkills'<?=selectcheck($filterby,'teamkills');?>Team Kills</option>
				<option value='timeplayed'<?=selectcheck($filterby,'timeplayed');?>Time Played (in sec)</option>
				<option value='connects'<?=selectcheck($filterby,'connects');?>Connections</option>
			</select><BR>
			&nbsp;&nbsp;<select name="FComp">
				<option value="<="<?=selectcheck($filtercompare,'<=');?>Less than or equal</option>
				<option value="<"<?=selectcheck($filtercompare,'<');?>Less than</option>
				<option value="="<?=selectcheck($filtercompare,'=');?>Equal to</option>
				<option value=">"<?=selectcheck($filtercompare,'>');?>Greater than</option>
				<option value=">="<?=selectcheck($filtercompare,'>=');;?>Greater than or equal</option>-->
			</select>&nbsp;&nbsp;<BR>
			<input type="text" name="FVal" value=<?echo $filtervalue;?>>
		</td>
		<td align=center valign=middle>
			<select name="Limit">
				<option value=10 <? if ($limit==10) echo "selected='true'";?>>10</option>
				<option value=25 <? if ($limit==25) echo "selected='true'";?>>25</option>
				<option value=50 <? if ($limit==50) echo "selected='true'";?>>50</option>
				<option value=100 <? if ($limit==100) echo "selected='true'";?>>100</option>
			</select>
			<P><input type="reset">
			<BR><input type="submit" value="Lookup">
			<input type="hidden" name="Prefix" value="<? echo $prefix; ?>">
		</td>
	</tr>
	</table>
	</form>

</td></tr>
</table>
<?
//Convert chosen column names and filter into queriable format for upcoming SELECT
$num_cols=count($fieldarray);
if ($num_cols>0) {
	for ($i=0; $i<$num_cols; $i++) {
		$querysegment=queryformat($fieldarray[$i]);
		if ($i==0 and $querysegment != '') $columnquery = queryformat($fieldarray[$i]);
		elseif ($i>0 and $querysegment != '') $columnquery .= ", ".$querysegment;
	}
}
else $columnquery="kills";

if ($filterby=='none' or $filterby==Null){
	$wherequery = '';
	$filter='';
}
else {
	$wherequery = "WHERE (".$filterby." ".urldecode($filtercompare)." ".$filtervalue.")";
	$filter=", filter: ".columnformat($filterby)." ".urldecode($filtercompare)." ".$filtervalue;
}
//Query requested info from the database and setup table for displaying it
$query = "SELECT name, playerid, avatarid, $columnquery FROM scorched3d".$prefix."_players $wherequery ORDER BY $orderby $dir LIMIT $playerid, $limit";
$result = mysql_query($query) or die("Query failed : " . mysql_error());
?>
<table width=760 border="0" align="center">
<tr><td align=center><font size="+1"><b><? echo "Players ".($playerid+1)." to ".($playerid+$limit)." (sorted by ".columnformat($orderby).$filter.")";?></b></font></td></tr>
</table>
<table width=760 bordercolor=#333333 cellspacing="0" cellpadding="0" border="1" align="center">
<tr>
<td bgcolor=#111111 width=50></td>

<?
//Display field names in the table with appropriate links for sorting the data
echo "<td bgcolor=#111111><b><a href=".$urlinfo."&PlayerID=0&OrderBy=name&Dir=".sortdirection('name', $orderby, $dir).">Player Name</a></b></td>";
for ($i=0; $i<$num_cols; $i++){
	echo "<td bgcolor=#111111 align=center><b><a href=".$urlinfo."&PlayerID=0&OrderBy=".$fieldarray[$i]."&Dir=".sortdirection($fieldarray[$i], $orderby, $dir).">".columnformat($fieldarray[$i])."</a></b></td>";
}
echo "</tr>";

//Format and display data in the appropriate fields
$rownum=0;
while ($row = mysql_fetch_array($result, MYSQL_ASSOC))
{
	++$rownum;
	echo "<tr>";
	echo "<td align=center>".($rownum + $playerid)."</td>";

	echo "<td>";
	echo "<table><tr>";
	echo "<td align=center><img border=0 src='getbinary.php?id=".$row[avatarid]."'></td>";
	echo "<td>";
	echo "<a href=\"playerstats.php?Prefix=".$prefix."&PlayerID=".$row[playerid]."\">";
	echo $row[name];
	echo "</a>";
	echo "</td>";
	echo "</tr></table>";
	echo "</td>";
	for ($i=0; $i<$num_cols; $i++){
		$rowname=$fieldarray[$i];
	    echo dataformat($rowname, $row[$rowname]);
	}
	echo "</tr>";
}
?>
</table>

<table width=760 align=center border=0> 
<tr><td>
<table border=0 align=center> 
<tr>

<?
//Count users and create links to all results
if ($num_rows==Null){
	//$query = "SELECT playerid FROM scorched3d".$prefix."_players $wherequery";
	//$result = mysql_query($query);
	//$num_rows = mysql_num_rows($result);
	$query = "select count(playerid) from scorched3d".$prefix."_players $wherequery";
	$result = mysql_query($query) or die("Query failed : " . mysql_error());
	$row = mysql_fetch_array($result);
	$num_rows=$row[0];
}
$pages = $num_rows / $limit;
$rows = 0;
if (gettype($pages)!="integer") $pages++;
for ($i=1; $i<=$pages; $i++)
{
	$startplayer = ($i - 1) * $limit + 1;
	$playerindex = $startplayer - 1;
	$endplayer = $startplayer + ($limit-1);
	echo "<td align=center><font size=-2><a href=".$urlinfo."&Prefix=".$prefix."&PlayerID=".$playerindex."&Players=".$num_rows."&OrderBy=".$orderby."&Dir=".$dir.">[$startplayer-$endplayer]</a></font></td>";
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
