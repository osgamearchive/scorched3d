<?
$orderby=$_GET['OrderBy'];
if ($orderby==Null)	$orderby="kills";
$dir=$_GET['Dir'];
if ($dir==Null)	$dir="desc";
$prefixid=$_GET['Prefix'];
$seriesid=$_GET['Series'];

include('statsheader.php');
include('sortfunction.php');
include('conversionfunctions.php');
?>

<?
// Top weapons query
$query="SELECT *, COALESCE(round(100*kills/shots, 2), 0.0) as killratio from scorched3d_weapons WHERE prefixid=".$prefixid." AND seriesid=".$seriesid." AND (scorched3d_weapons.cost) >= 0 GROUP BY weaponid ORDER BY $orderby $dir";
$result = mysql_query($query) or die("Query failed : " . mysql_error() ."<br>".$query);
?>
<table width="600" border="0" align="center">
<tr><td align=center><font size="+1"><b><? echo $mod_name." Weapons"; ?></b></font></td></tr>
</table>
<table width="600" bordercolor=#333333 cellspacing="0" border="1" align="Center">
<tr>
<td bgcolor=#111111 width=35></td>
<?
echo "<td bgcolor=#111111 align=center><b><a href=allweapons.php?Prefix=$prefixid&Series=$seriesid&OrderBy=name&Dir=".sortdirection('name', $orderby, $dir).">Weapon</a></b></td>";
echo "<td bgcolor=#111111 align=center><b><a href=allweapons.php?Prefix=$prefixid&Series=$seriesid&OrderBy=kills&Dir=".sortdirection('kills', $orderby, $dir).">Kills</a></b></td>";
echo "<td bgcolor=#111111 align=center><b><a href=allweapons.php?Prefix=$prefixid&Series=$seriesid&OrderBy=killratio&Dir=".sortdirection('killratio', $orderby, $dir).">Killratio</a></b></td>";
echo "<td bgcolor=#111111 align=center><b><a href=allweapons.php?Prefix=$prefixid&Series=$seriesid&OrderBy=armslevel&Dir=".sortdirection('armslevel', $orderby, $dir).">Arms Level</a></b></td>";
echo "<td bgcolor=#111111 align=center><b><a href=allweapons.php?Prefix=$prefixid&Series=$seriesid&OrderBy=cost&Dir=".sortdirection('cost', $orderby, $dir).">Cost</a></b></td>";
echo "<td bgcolor=#111111 align=center><b><a href=allweapons.php?Prefix=$prefixid&Series=$seriesid&OrderBy=bundlesize&Dir=".sortdirection('bundlesize', $orderby, $dir).">Bundle Size</a></b></td>";
echo "<td bgcolor=#111111 align=center><b><a href=allweapons.php?Prefix=$prefixid&Series=$seriesid&OrderBy=shots&Dir=".sortdirection('shots', $orderby, $dir).">Shots</a></b></td>";
//<td bgcolor=#111111 align=center><b>Description</b></td>
?>
</tr>
<?
$rownum=0;	
while ($row = mysql_fetch_object($result))
{
	++$rownum;	
	echo "<tr>";
	echo "<td align=center>$rownum</td>";
	echo "<td><a href=\"weaponstats.php?Prefix=$prefixid&Series=$seriesid&WeaponID=$row->weaponid\" title=\"$row->description\">$row->name</a></td>";
	echo "<td align=center>$row->kills</td>";
	echo "<td align=center>".prependnumber($row->killratio, "%", 1)."</td>";
	echo "<td align=center>$row->armslevel</td>";
	echo "<td align=center>".prependnumber($row->cost, "$", 0)."</td>";
	echo "<td align=center>$row->bundlesize</td>";
	echo "<td align=center>$row->shots</td>";
	//echo "<td>$row->description</td>";
	echo "</tr>";
}
?>
</table><br>

<?
include('statsfooter.php');
?>

</body>
</html>