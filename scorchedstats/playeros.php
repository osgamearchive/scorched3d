<?
include('statsheader.php');
?>

<?
$query = " select count(*) as cnt FROM scorched3d_players";
$result = mysql_query($query) or die("Query failed : " . mysql_error());
$row = mysql_fetch_object($result);
$total = $row->cnt;
$other = $row->cnt;
?>

<table width="640" border="0" align="center">
<tr><td align=center><b>Player Operating Systems</b></td></tr>
<tr><td align=center>(Note: These stats are gathered only from online players using the official servers)</td></tr>
</table>
<br>
<table width="600" bordercolor="#333333" cellspacing="0" cellpadding="0" border="1" align="center">
<tr>
<td bgcolor=#111111><center><b>OS</b></center></td>
<td bgcolor=#111111><center><b>Players</b></center></td>
<td width=200 bgcolor=#111111><center><b>Percentage</b></center></td>
</tr>
<?
$arr = array(
	"Windows NT/2000/XP" => "Windows NT%", 
	"Windows ME/98" => "Windows 98%", 
	"Darwin" => "Darwin%", 
	"Linux" => "Linux%"
);

foreach ($arr as $key => $value) {
	$query = " select count(*) as cnt FROM scorched3d_players where osdesc LIKE '$value'";
	$result = mysql_query($query) or die("Query failed : " . mysql_error());
	$row = mysql_fetch_object($result);
	$windows = $row->cnt;
	$per = round($windows / $total * 100, 1);
	$other = $other - $windows;
	$w = round($per, 0);
	$w2 = 100 - $w;
	
	echo "<tr><td>$key</td><td>$windows</td><td><img src=green.gif width=$w height=10 border=0><img src=grey.gif width=$w2 height=10 border = 0>&nbsp;$per %</td></tr>";
}

$windows = $other;
$per = round($windows / $total * 100, 1);
$w = round($per, 0);
$w2 = 100 - $w;
$key="Others";
echo "<tr><td>$key</td><td>$windows</td><td><img src=green.gif width=$w height=10 border=0><img src=grey.gif width=$w2 height=10 border = 0>&nbsp;$per %</td></tr>";
?>
</table>
<?
include('statsfooter.php');
?>

