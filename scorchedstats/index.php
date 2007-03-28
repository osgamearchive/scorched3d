<?
include('statsheader.php');
include('conversionfunctions.php')
?>

<?
$seriesid = getIntParameter('Series');
if ($seriesid==0)
{
	$query=" select seriesid, rounds, games, started, ended, name from scorched3d_series where type = 0;";
	$result = mysqlQuery($query) or die("Query 1 failed : " . mysql_error());
	$seriesrow = mysql_fetch_object($result);
}
else
{
	$query=" select seriesid, rounds, games, started, ended, name  from scorched3d_series where seriesid = ".$seriesid;
	$result = mysqlQuery($query) or die("Query 1 failed : " . mysql_error());
	$seriesrow = mysql_fetch_object($result);
}

if (!$seriesrow)
{
include('index-nocurrent.php');
}
else
{
include('index-current.php');
}

?>


<p align=center>
<?
include('search.php');
?>
</p>

<?
include('statsfooter.php');
?>
