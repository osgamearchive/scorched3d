<?
include('statsheader.php');
?>

<?
$fp = gzopen("../upload/backup.sql.gz", "r");
if (!fp) die("Failed to open upload file");
$sql = "";
$affected = 0;
$count = 0;
$currenttime = time();
while (!feof ($fp))
{
	$line = fgets($fp, 128048);
	if ($line[0] != '-')
	{
		$sql .= $line;

		if (strpos($sql, ";") != FALSE)
		{
			//print $count . ">>>".$sql."<br>";

			if ($count % 50 == 0) print "<br>";
			print ".";

			{
				mysql_unbuffered_query($sql) or die("Query failed : ".$sql.":" . mysql_error());
				$affected += mysql_affected_rows();
			}

			$sql = "";
			$count ++;
			//if ($count > 20) break;
		}
	}

}
fclose($fp);

$totaltime = time() - $currenttime;

print "Upload successful : <br>";
print "  Affected db lines : ".$affected."<br>";
print "  Query lines : ".$count."<br>";
print "  Time in seconds : ".$totaltime."<br>";

?>

<?
include('statsfooter.php');
?>
