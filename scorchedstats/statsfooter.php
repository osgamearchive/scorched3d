<p align=center>
<?
        $query=" select (UNIX_TIMESTAMP() - UNIX_TIMESTAMP(eventtime)) as lasttime from scorched3d_events order by eventtime desc limit 1";
        $result = mysql_query($query) or die("Query failed : " . mysql_error());
        if ($row = mysql_fetch_object($result))
        {
                $eventtimesecs = $row->lasttime + 0;
                $eventtimemins = intval($eventtimesecs / 60) % 60;
                $eventtimehours = intval($eventtimesecs / 60 / 60);
                $eventtimesecs = $eventtimesecs % 60;
                echo "These stats were updated $eventtimehours hours, $eventtimemins mins, $eventtimesecs secs ago.<br>";
        }
?>

<a href="/phpBB2/viewtopic.php?p=2935#2935">
Why do I appear in the stats more than once?</a>
</p>

<?
include('../footer.php');
?>

</body>
</html>

