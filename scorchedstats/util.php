<?
	$seriesquery=" select seriesid, rounds, games, started, ended, name  from scorched3d_series where seriesid = ".$seriesid;
	$seriesresult = mysql_query($seriesquery) or die("Query 1 failed : " . mysql_error());
	$seriesrow = mysql_fetch_object($seriesresult);

	$sourcequery =" select serverid from scorched3d_statssource where seriesid = ".$seriesid." and prefixid = ".$prefixid;
	$sourceresult = mysql_query($sourcequery) or die("Query 2 failed : " . mysql_error());
	while ($sourcerow = mysql_fetch_object($sourceresult))
	{
		$serverid = $sourcerow->serverid;

		$serverresult = mysql_query("select name from scorched3d_servers where serverid = ".$serverid) or die("Query failed : " . mysql_error());
		$serverrow = mysql_fetch_array($serverresult);
		$server = $serverrow[0];
	}
?>

<table width="790" border="0" align="center">
<tr align=center>
<td>
<font size=+2><b><u>Stats for <?=$seriesrow->name?></u></b></font><br>
<b>Server : <?=$server?></b>
</td>
</tr>
</table>
