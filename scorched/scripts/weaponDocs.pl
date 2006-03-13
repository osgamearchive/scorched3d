#!/usr/local/bin/perl

use strict;

opendir(DIR, ".") || die "ERROR: Cannot open dir";
my @files = grep { /cpp$/ } readdir(DIR);
closedir(DIR);

foreach my $file (@files)
{
	parseFile($file);
}

sub parseFile
{
	my ($file) = @_;
	
	open (FILE, "$file") || die "ERROR: Cannot open file $file";
	my @fileconts = <FILE>;
	close (FILE);	
	
	my @fns = grep { /\s+(\w+)::parseXML/ } @fileconts;
	next if ($#fns == -1);
	$fns[0] =~ /\s+(\w+)::parseXML/ or die "ERROR1";
	my $class = $1;
	print "--".$class."--\n";
	
	my $dir = "";
	my @dirfns = grep { /\!(\w+)::parseXML/ } @fileconts;
	if ($#dirfns != -1)
	{
		$dirfns[0] =~ /\!(\w+)::parseXML/ or die "ERROR2";
		my $dir = $1;
		print "  Extends : ".$dir."\n";		
	}
	elsif ($file !~ /Accessory/)
	{
		print "  Extends : Accessory\n";
	}
	
	my @nodes = grep { /getNamedChild/ } @fileconts;
	foreach my $node (@nodes)
	{
		if ($node =~ /getNamedChild\(\"(\w+)\"\s*,\s*([\w_]+)(\s*,\s*false|)/)
		{
			my ($nodename, $var, $rest) = ($1, $2, $3);
			
			my $fileh = $file;
			$fileh =~ s/cpp$/h/;
			my $type = getType($fileh, $nodename);
			$type = getType($fileh, $var) if ($type eq "");
			$type = getType($file, $var) if ($type eq "");
			
			$rest = "(optional)" if ($rest ne "");
			
			print "  Attribute : ".$nodename.", Type : ".$type." ".$rest."\n";
		}
	}
}

sub getType
{
	my ($file, $var) = @_;
	
	return "float" if (length($var) == 1);
	
	open (FILE, "$file") || die "ERROR: Cannot open file $file";
	my @fileconts = <FILE>;
	close (FILE);		
	
	my @types = grep { /$var[_,\s;]+/i } @fileconts;
	foreach (@types)
	{
		if (/^\s*([_\*\w:]+)/i)
		{
			return $1 if ($1 eq "float");
			return $1 if ($1 eq "bool");
			return $1 if ($1 eq "std::string");
			return $1 if ($1 eq "Vector");
			return $1 if ($1 eq "int");
		}
	}
	return "";
}
