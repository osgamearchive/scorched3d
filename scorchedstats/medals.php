<?php

function get_medal($playerid, $rank, $small, $prefix, $series, $name)
{
	$image = '';
	if ($rank + 0 == 1) 
	{
		$image = '/stats/images/rank1.jpg';
		$text = 'Previously Ranked 1st';
	}
	else if ($rank + 0 == 2) 
	{
		$image = '/stats/images/rank2.jpg';
		$text = 'Previously Ranked 2nd';
	}
	else if ($rank + 0 == 3) 
	{
		$image = '/stats/images/rank3.jpg';
		$text = 'Previously Ranked 3rd';
	}
	else if ($rank + 0 <= 10)
	{
		$image = '/stats/images/gold-blk.jpg';
		$text = 'Previous Top 10 Placing';
	}
	else if ($rank + 0 <= 25)
	{
		$image = '/stats/images/silver-blk.jpg';
		$text = 'Previous Top 25 Placing';
	} 
	else if ($rank + 0 <= 50)
	{
		$image = '/stats/images/bronze-blk.jpg';
		$text = 'Previous Top 50 Placing';
	}
	else
	{
		$image = '/stats/images/star-blk.jpg';
		$text = 'Previously Placed ' + ($rank + 0);
	}
						
	$imgtext = 
		"<a href='/stats/playerstats.php?Prefix=".$prefix."&Series=".
			$series."&PlayerID=".
			$playerid."' title='".$text." : Ranked ".
			$rank." in ".
			$name."'>".
			"<img border=0 src='".$image."'></a>";
					
	if ($small)
	{
		$imgtext = str_replace(".jpg", "-small.jpg", $imgtext);
	}

	return $imgtext;
}

?>
