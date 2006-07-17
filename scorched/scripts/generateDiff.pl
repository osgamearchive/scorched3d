use strict;
use File::Compare;

if (!defined $ARGV[0])
{
	die "Usage : $0 <newdir> <olddir>";
}

my ($dir1, $dir2) = @ARGV;

checkDir($dir1, $dir2);

sub checkDir()
{
	my ($dir1, $dir2) = @_;

	# Src dir
	opendir(DIR, $dir1) || die "ERROR";
	my @files = readdir(DIR);
	closedir(DIR);

	foreach my $file (@files)
	{
		my $file1 = $dir1."\\".$file;
		my $file2 = $dir2."\\".$file;

		if (-f $file1)
		{
			if (! -f $file2)
			{
				# New file
				print "New : $file1\n";
			}
			else
			{
				if (compare($file1, $file2) != 0)
				{
					# Different file
					print "Diff : $file1\n";
				}
			}
		}
		elsif (-d $file1 && $file !~ /^\./)
		{
			checkDir($file1, $file2);
		}
	}
	
	if (-d $dir2)
	{
		# Dest dir
		opendir(DIR, $dir2) || die "ERROR";
		my @files = readdir(DIR);
		closedir(DIR);

		foreach my $file (@files)
		{
			my $file1 = $dir1."\\".$file;
			my $file2 = $dir2."\\".$file;

			if (-f $file2)
			{
				if (! -f $file1)
				{
					# Old file
					print "Old : $file2\n";
				}
			}
			elsif (-d $file2 && $file !~ /^\./)
			{
				if (! -d $file1)
				{
					# Old dir
					print "Old Dir : $file2\n";				
				}
			}
		}
	}
}
