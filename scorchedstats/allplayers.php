<?
$playerid=$_GET['PlayerID'];
if ($playerid==Null) $playerid=0;
$prefix=$_GET['Prefix'];
if ($prefix==Null)	$prefix="";
$orderby=$_GET['OrderBy'];
if ($orderby==Null)	$orderby="kills";
$dir=$_GET['Dir'];
if ($dir==Null)	$dir="desc";
include('statsheader.php');
include('sortfunction.php');
include('conversionfunctions.php')
?>
                                                                                            
<table width=640 border="0" align="center">
<tr><td align="center">
<?
include('search.php');
?>
</td></tr>
</table>

<?
$query = " SELECT wins, (scorched3d".$prefix."_players.overallwinner) as gamewins, skill + wins * 150 + overallwinner * 500 as skill, playerid, kills, lastconnected, shots, name, (scorched3d".$prefix."_players.selfkills) as suicides, teamkills, resigns, (scorched3d".$prefix."_players.gamesplayed) as roundsplayed, timeplayed, moneyearned FROM scorched3d".$prefix."_players ORDER BY $orderby $dir LIMIT $playerid, 25";
$result = mysql_query($query) or die("Query failed : " . mysql_error());
?>
<table width=640 border="0" align="center">
<tr><td align=center><font size="+1"><b><? echo "Players ".($playerid+1)." to ".($playerid+25)." (sorted by ".$orderby.")";?></b></font></td></tr>
</table>
<table width=640 bordercolor=#333333 cellspacing="0" cellpadding="0" border="1" align="center">
<tr>
<td bgcolor=#111111 width=50></td>

<?
echo "<td bgcolor=#111111><b><a href=allplayers.php?Prefix=$prefix&PlayerID=0&OrderBy=name&Dir=".sortdirection('name', $orderby, $dir).">Player Name</a></b></td>";
echo "<td align=center bgcolor=#111111><b><a href=allplayers.php?Prefix=$prefix&?PlayerID=0&OrderBy=skill&Dir=".sortdirection('skill', $orderby, $dir).">Skill</a></b></td>";
echo "<td align=center bgcolor=#111111><b><a href=allplayers.php?Prefix=$prefix&?PlayerID=0&OrderBy=kills&Dir=".sortdirection('kills', $orderby, $dir).">Kills</a></b></td>";
//echo "<td align=center bgcolor=#111111><b><a href=allplayers.php?Prefix=$prefix&?PlayerID=0&OrderBy=shots&Dir=".sortdirection('shots', $orderby, $dir).">Shots</a></b></td>";
echo "<td align=center bgcolor=#111111><b><a href=allplayers.php?Prefix=$prefix&?PlayerID=0&OrderBy=moneyearned&Dir=".sortdirection('moneyearned', $orderby, $dir).">Money Earned</a></b></td>";
//echo "<td align=center bgcolor=#111111><b><a href=allplayers.php?Prefix=$prefix&?PlayerID=0&OrderBy=roundsplayed&Dir=".sortdirection('roundsplayed', $orderby, $dir).">Rounds</a></b></td>";
echo "<td align=center bgcolor=#111111><b><a href=allplayers.php?Prefix=$prefix&?PlayerID=0&OrderBy=wins&Dir=".sortdirection('wins', $orderby, $dir).">Rnd Wins</a></b></td>";
echo "<td align=center bgcolor=#111111><b><a href=allplayers.php?Prefix=$prefix&?PlayerID=0&OrderBy=gamewins&Dir=".sortdirection('gamewins', $orderby, $dir).">Game Wins</a></b></td>";
echo "<td align=center bgcolor=#111111><b><a href=allplayers.php?Prefix=$prefix&?PlayerID=0&OrderBy=resigns&Dir=".sortdirection('resigns', $orderby, $dir).">Resigns</a></b></td>";
echo "<td align=center bgcolor=#111111><b><a href=allplayers.php?Prefix=$prefix&?PlayerID=0&OrderBy=suicides&Dir=".sortdirection('suicides', $orderby, $dir).">SKs</a></b></td>";
echo "<td align=center bgcolor=#111111><b><a href=allplayers.php?Prefix=$prefix&?PlayerID=0&OrderBy=teamkills&Dir=".sortdirection('teamkills', $orderby, $dir).">TKs</a></b></td>";
echo "<td align=center bgcolor=#111111><b><a href=allplayers.php?Prefix=$prefix&?PlayerID=0&OrderBy=timeplayed&Dir=".sortdirection('timeplayed', $orderby, $dir).">Time Played</a></b></td>";
//echo "<td width=160 bgcolor=#111111><b><a href=allplayers.php?Prefix=$prefix&?PlayerID=0&OrderBy=lastconnected&Dir=".sortdirection('lastconnected', $orderby, $dir).">Last Connect</b></td>";
?>
</tr>
<?
$rownum=0;
while ($row = mysql_fetch_object($result))
{
        ++$rownum;
        if ($row->name==Null)
        	$name = 'NULL';
        else
        	$name = $row->name;
        echo "<tr>";
        echo "<td align=center>".($rownum + $playerid)."</td>";
        echo "<td><a href=playerstats.php?Prefix=$prefix&PlayerID=$row->playerid>".$name."</a></td>";
        echo "<td align=center>$row->skill</td>";
        echo "<td align=center>$row->kills</td>";
        //echo "<td align=center>$row->shots</td>";
     	echo "<td align=right>".prependnumber($row->moneyearned, "$", 0)."</td>";
        //echo "<td align=center>$row->roundsplayed</td>";
        echo "<td align=center width=50>$row->wins</td>";
        echo "<td align=center width=50>$row->gamewins</td>";
        echo "<td align=center width=50>$row->resigns</td>";
        echo "<td align=center>$row->suicides</td>";
        echo "<td align=center>$row->teamkills</td>";
     	echo "<td align=right>".secondstotext($row->timeplayed)."</td>";
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
$query = "select count(playerid) from scorched3d".$prefix."_players;";
$result = mysql_query($query) or die("Query failed : " . mysql_error());
$row = mysql_fetch_array($result);
$pages = $row[0] / 25 ;
$rows = 0;
if (gettype($pages)!="integer") $pages++;
for ($i=1; $i<=$pages; $i++)
{
	$startplayer = ($i - 1) * 25 + 1;
	$playerindex = $startplayer - 1;
	$endplayer = $startplayer + 24;
	echo "<td align=center><font size=-2><a href=allplayers.php?Prefix=$prefix&PlayerID=$playerindex&OrderBy=$orderby&Dir=$dir>[$startplayer-$endplayer]</a></font></td>";
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
