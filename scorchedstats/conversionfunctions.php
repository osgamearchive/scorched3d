<?
// Contains functions used to convert raw data from the database into a more readable format

// Converts a number of seconds into a text format
function secondstotext($timeinseconds) {
	if ($timeinseconds>=86400) $days=floor($timeinseconds/86400);
	else $days=0;
	
	$timeinseconds=$timeinseconds-($days*86400);
	if ($timeinseconds>=3600) $hours=floor($timeinseconds/3600);
	else $hours=0;
	
	$timeinseconds=$timeinseconds-($hours*3600);
	if ($timeinseconds>=60) $minutes=floor($timeinseconds/60);
	else $minutes=0;
	
	$seconds=$timeinseconds-($minutes*60);
	
	$timeintext="";
	if ($days>0) $timeintext.=$days."d ";
	if ($hours>0) $timeintext.=$hours."h ";
	if ($minutes>0) $timeintext.=$minutes."m ";
	if ($seconds>0) $timeintext.=$seconds."s";
	
	return $timeintext;
}

// Prepends a number with a symbol, adds commas and returns a specific number of decimal places
function prependnumber ($numtoconvert, $prependsymbol, $decimalplaces) {
	if (!is_numeric($numtoconvert)) return $prependsymbol.number_format(0, $decimalplaces);
	elseif ($numtoconvert >= 0){
		$convertednum=$prependsymbol.number_format($numtoconvert, $decimalplaces);
		return $convertednum;
	}
	elseif ($numtoconvert < 0){
		$numtoconvert=abs($numtoconvert);
		$convertednum="-".$prependsymbol.number_format($numtoconvert, $decimalplaces);
		return $convertednum;
	}
}
?>