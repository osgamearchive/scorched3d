<?
	$seriesquery=" select seriesid, type, rounds, games, started, ended, name  from scorched3d_series where seriesid = ".$seriesid;
	$seriesresult = mysqlQuery($seriesquery) or die("Query 1 failed : " . mysql_error());
	$seriesrow = mysql_fetch_object($seriesresult);

	$sourcequery =" select serverid from scorched3d_statssource where seriesid = ".$seriesid." and prefixid = ".$prefixid;
	$sourceresult = mysqlQuery($sourcequery) or die("Query 2 failed : " . mysql_error());

	$displaystats = 0;
	$server = "";
	$type = $seriesrow->type;
	while ($sourcerow = mysql_fetch_object($sourceresult))
	{
		$serverid = $sourcerow->serverid;

		$serverresult = mysqlQuery("select name, displaystats from scorched3d_servers where serverid = ".$serverid) or die("Query failed : " . mysql_error());
		$serverrow = mysql_fetch_object($serverresult);
		$server = $serverrow->name;
		$displaystats = $serverrow->displaystats;
	}
	
	if ($displaystats == 0)
	{
		die("Cannot show stats for this server");
	}
?>

<table width="790" border="0" align="center">
<tr align=center>
<td>
<font size=+2><b><u>Stats for <?=$seriesrow->name?></u></b></font><br>
<b>Server : <?=$server?></b><br>
<i>
<?
if ($type == 0) print "Note: This stats series is still in progress.";
else print "Note: This stats series has completed.";
?>
</i>
</td>
</tr>
</table>
