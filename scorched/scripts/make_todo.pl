#!/usr/bin/perl

$infile = "..\\TODO";
$outfile = "..\\documentation\\todo.html";

open(INPUTFILE, $infile) || die "cannot open $infile for reading: $!";
open(OUTPUTFILE, ">$outfile") || die "cannot create $outfile for writing: $!";

&print_header;

while ($todo = <INPUTFILE>) {
    &Chop(*todo);
    
    next if ( $todo =~ /^\#/);
    if ( $todo =~ s/^--//) {
        print OUTPUTFILE ("<h2>$todo</h2>\n");
    } else {
        print OUTPUTFILE ("$todo<br>\n");
    }
}

&print_footer;

close(INPUTFILE);
close(OUTPUTFILE);

sub print_header {

print OUTPUTFILE <<ENDOFHTML
<!DOCTYPE HTML PUBLIC "-//W3C//DTD HTML 4.01 Transitional//EN">
<html>
<head>
<title>Scorched 3D - A 3D Remake Of Scorched Earth</title>
<meta http-equiv="Content-Type" content="text/html; charset=iso-8859-1">
<meta name="description" content="Scorched 3D a 3D update of Scorched Earth">
<meta name="keywords" content="Scorched Earth,Scorched,Earth,Landscape,Terrain,OpenGL,ROAM,3D">
</head>

<body bgcolor="#000000" text="#FFFFFF" link="#FF9933" vlink="#FF9933" alink="#FF9933">
<table border="0">
  <tr>
    <td width="700" valign="top" align="center">
	<img src="buttons/scorched2.jpg">
    </td>
  </tr>
  <tr>
    <td width="700" valign="top">
	  <table align="center" cellpadding=0 cellspacing=0 border="0">
        <tr> 
          <td><a href="index.html"><img src="buttons/home.jpg" border="0"></td>
          <td><a href="forums/news.cgi"><img src="buttons/news.jpg" border="0"></a></td>
          <td><a href="renderer.html"><img src="buttons/renderer.jpg" border="0"></a></td>
          <td><a href="screens.html"><img src="buttons/screenshots.jpg" border="0"></a></td>
          <td><a href="downloads.html"><img src="buttons/downloads.jpg" border="0"></a></td>
          <td><a href="links.html"><img src="buttons/links.jpg" border="0"></a></td>
          <td><a href="forums/main.cgi"><img src="buttons/forums.jpg" border="0"></a></td>
        </tr>
	<tr>
		<td align=center><font color=#ff9933>Home</font></td>
		<td align=center>News</td>
		<td align=center>Technology</td>
		<td align=center>Screen Shots</td>
		<td align=center>Downloads</td>
		<td align=center>Links</td>
		<td align=center>Forums</td>
	</tr>
      </table>
      <p align="center"><img src="buttons/spacer.JPG" width="400" height="6"></p>
	</td>
  </tr>
  <tr>
    <td width="700" valign="top">
	<br>
ENDOFHTML
}

sub print_footer {

print OUTPUTFILE <<ENDOFHTML
	<br>
    </td>
  </tr>
  <tr>
    <td width="700"> 
		<p align="center"><img src="buttons/spacer.JPG" width="400" height="6"></p>
      	<table width="500" border="0" align="center">
        <tr>
          <td width="80">&nbsp;</td>
          <td align="center" width="300"><font size="-2"><a href="mailto:gcamp\@cisco.com">Gavin C.</a> 
		  	is currently working for:<br>
            <a href="http://www.cisco.com">Cisco Systems</a><br>
            All work on these pages is<br>
            &copy; Copyright 2002 G. Camp. All rights reserved. </font></td>
          <td align="center" width="80">
			  <a href="http://www.opengl.org"><img src="buttons/OGLonBlackSm.jpg" width="68" height="30" border="0"></a>
		  </td>
        </tr>
      </table>
	</td>
  </tr>
</table>
</body>
</html>
ENDOFHTML
}


############################################################
##
## Chop(string) 
##
## Does same job as regular Perl chop(string), but will
## chop a "\n\r" at the end of the line, not just the "\n"
##
## NOTE: Will not work with no argument 
##       (ie. &Chop; will NOT chop $_)
##
############################################################
sub Chop {
    local(*string_to_chop) = @_;
    chop $string_to_chop;
    chop $string_to_chop if ($string_to_chop =~ /\r$/);
}

