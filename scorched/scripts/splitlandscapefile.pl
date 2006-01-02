use strict;

my ($infile, $tag) = @ARGV;
if (!defined $infile)
{
	print "Usage : $0 <infile> <splittag>";
	exit 1;
}

open(IN, $infile) || die "ERROR: Cannot open file $infile";
my @filelines = <IN>;
close(IN);

my ($content, $name) = ("", "");
foreach (@filelines)
{
	s/\t//;
	if (/<name>(\w+)<\/name>/)
	{
		if ($name eq "")
		{
			$name = $1;
		}
		else
		{
			$content .= $_;
		}
	}
	elsif (/<$tag>/)
	{
		$name = "";
		$content = $_;
	}
	else 
	{
		$content .= $_;
	}

	if (/<\/$tag>/)
	{
		my $outfile = $tag.$name.".xml";
		open(OUT, ">$outfile") || die "ERROR: Cannot write $outfile";
		print OUT $content;
		close(OUT);
	}
}
