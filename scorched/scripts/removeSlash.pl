use strict;

my @dirs =
(
	"3dsparse",
	"actions",
	"common",
	"coms",
	"dialogs",
	"engine",
	"GLEXT",
	"GLW",
	"ode",
	"landscape",
	"client",
	"server",
	"tankgraph",
	"sprites",
	"tank",
	"weapons"
);

my $dir;
foreach $dir (@dirs)
{
	opendir(IN, "..\\src\\$dir") || die "ERROR: DIR \"$dir\"";
	my @files = grep { /\.h/ || /\.cpp/ } readdir(IN);
	closedir(IN);

	my $file;
	foreach $file (@files)
	{
		open (INFILE, "..\\src\\$dir\\$file") || die "ERROR: File \"..\\$dir/$file\"";
		my @filelines = <INFILE>;
		close (INFILE);

		my @newfilelines = ();
		my $save = undef;
		for my $line (@filelines)
		{
			if ($line =~ /#include/)
			{
				if ($line =~ /\\/)
				{
					$save = 1;
					$line =~ s/\\/\//g;
				}
			}

			push @newfilelines, $line;
		}

		if (defined $save)
		{
			print $file."\n";
			open (OUTFILE, ">..\\src\\$dir\\$file") || die "ERROR: File \"..\\$dir/$file\"";
			print OUTFILE @newfilelines;
			close (OUTFILE);			
		}
	}
}