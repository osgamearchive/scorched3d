<?
include('statsheader.php');
include('conversionfunctions.php')
?>

<?
$sql = "SELECT * FROM `scorched3d_statssource` ".
	"LEFT JOIN scorched3d_series ON scorched3d_series.seriesid = scorched3d_statssource.seriesid " .
	"LEFT JOIN scorched3d_prefixs ON scorched3d_prefixs.prefixid = scorched3d_statssource.prefixid";
$result = mysql_query($sql) or die("Query failed : " . mysql_error());

while ($row = mysql_fetch_object($result))
{
	$sql = 'SELECT playerid, kills from scorched3d_stats where seriesid = '.$row->seriesid.' and prefixid = '.$row->prefixid.' order by kills desc';
	$subresult = mysql_query($sql) or die("Query failed : " . mysql_error());		
	$count = 1;
	while ($subrow = mysql_fetch_object($subresult))
	{
		$sql2 = 'UPDATE scorched3d_stats SET rank = '.$count.' WHERE seriesid = '.$row->seriesid.' and prefixid = '.$row->prefixid.' and playerid = '.$subrow->playerid;
		$result2 = mysql_query($sql2) or die("Query failed : " . mysql_error());
		$count++;
	}
}
?>

<?
include('statsfooter.php');
?>
