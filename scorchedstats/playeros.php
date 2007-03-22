<?
include('statsheader.php');
?>

<?
$query = " select count(*) as cnt FROM scorched3d_players";
$result = mysqlQuery($query) or die("Query failed : " . mysql_error());
$row = mysql_fetch_object($result);
$total = $row->cnt;
$other = $row->cnt;
?>

<table width="640" border="0" align="center">
<tr><td align=center><b>Player Operating Systems</b></td></tr>
<tr><td align=center>(Note: These stats are gathered only from online players using this server)</td></tr>
</table>
<table width="600" bordercolor="#333333" cellspacing="0" cellpadding="0" border="1" align="center">
<tr>
<td bgcolor=#111111><center><b>OS</b></center></td>
<td bgcolor=#111111><center><b>Players</b></center></td>
<td width=200 bgcolor=#111111><center><b>Percentage</b></center></td>
</tr>
<?
$arr = array(
	"Windows" => "Win%", 
	"Darwin" => "Darwin%", 
	"FreeBSD" => "FreeBSD%", 
	"Linux" => "Linux%"
);

foreach ($arr as $key => $value) {
	$query = " select count(*) as cnt FROM scorched3d_players where osdesc LIKE '$value'";
	$result = mysqlQuery($query) or die("Query failed : " . mysql_error());
	$row = mysql_fetch_object($result);
	$windows = $row->cnt;
	$per = round($windows / $total * 100, 1);
	$other = $other - $windows;
	$w = round($per, 0);
	$w2 = 100 - $w;
	
	echo "<tr><td>$key</td><td>$windows</td><td><img src=images/green.gif width=$w height=10 border=0><img src=images/grey.gif width=$w2 height=10 border = 0>&nbsp;$per %</td></tr>";
}

$windows = $other;
$per = round($windows / $total * 100, 1);
$w = round($per, 0);
$w2 = 100 - $w;
$key="Others";
echo "<tr><td>$key</td><td>$windows</td><td><img src=images/green.gif width=$w height=10 border=0><img src=images/grey.gif width=$w2 height=10 border = 0>&nbsp;$per %</td></tr>";
echo "<tr><td>Total Players</td><td>$total</td><td></td></tr>";
?>
</table>
<br><br>
<table width="640" border="0" align="center">
<tr><td align=center><b>Server Activity</b></td></tr>
</table>
<table width="600" bordercolor="#333333" cellspacing="0" cellpadding="0" border="1" align="center">
<tr>
<td bgcolor=#111111><center><b>Time</b></center></td><td bgcolor=#111111><center><b>Connects</center></b></td><td bgcolor=#111111></td>
</tr>
<?

for ($i=27; $i>=0; $i--)
{ 
	$start = $i * 21600;
	$end = ($i + 1) * 21600;
	$query = " select count(eventid) as cnt from scorched3d_events where eventtype=7 and UNIX_TIMESTAMP(NOW()) - UNIX_TIMESTAMP(eventtime) < $end and UNIX_TIMESTAMP(NOW()) - UNIX_TIMESTAMP(eventtime) >= $start;";
	$result = mysqlQuery($query) or die("Query failed : " . mysql_error());
	$row = mysql_fetch_object($result);
	$total = $row->cnt;

	echo "<tr>";
	echo "<td> Time -".($i * 4)." to -".(($i+1) * 4)."hours</td>\n";
	echo "<td>".$total."</td>\n";
	echo "<td><img src=images/green.gif width=".$total." height=10 border=0></td>\n";
	echo "</tr>";
}

?>
</table>

<?
include('statsfooter.php');
?>

