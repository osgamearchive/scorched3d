use strict;

my @dirs =
(
	"3dsparse",
	"common",
	"coms",
	"dialogs",
	"engine",
	"GLEXT",
	"GLW",
	"landscape",
	"client",
	"server",
	"actions",
	"scorched",
	"sprites",
	"tankai",
	"XML",
	"tankgraph",
	"tank",
	"weapons"
);

my $dir;
foreach $dir (@dirs)
{
	opendir(IN, "..\\src\\$dir") || die "ERROR: DIR \"$dir\"";
	my @files = grep { /\.h/ } readdir(IN);
	closedir(IN);

	my $file;
	foreach $file (@files)
	{
		open (INFILE, "..\\src\\$dir\\$file") || die "ERROR: File \"..\\$dir/$file\"";
		my @filelines = <INFILE>;
		close (INFILE);

		my $incline = "__INCLUDE_".$file."_INCLUDE__";
		$incline =~ s/\.//g;
		if ($filelines[0] =~ /pragma once/)
		{
			print "$dir/$file\n";

			$filelines[0] = "#if !defined($incline)\n#define $incline\n";
			push @filelines, "\n\n#endif\n";

			open(OUTFILE, ">..\\src\\$dir\\$file") || die "ERROR: Out";
			print OUTFILE @filelines;
			close (OUTFILE);
		}
	}
}