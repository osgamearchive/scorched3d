<?
function sortdirection($newsort, $oldsort, $olddir){
	if ($newsort == $oldsort){
		switch ($olddir){
			case "asc": $newdir="desc"; break;
			case "desc": $newdir="asc"; break;
		}
		return $newdir;
	}
	else{
		if ($newsort == "name")
			return "asc";
		else
			return "desc";
	}
}
?>