use strict;

my %usedFiles = ();

addVCProj("../src/scorched/scorched.vcproj", "scorched", "scorched");

sub addVCProj
{
	my %localfiles;
	my ($vcfile, $exename, $dirname) = @_;

	my $basedir = $dirname;

	open(IN, $vcfile) || die "ERROR: Open $vcfile";
	while(<IN>)
	{
		if (/RelativePath="([^"]+)"/)
		{
			my $file = $1;
			$file =~ s/\\/\//g;
			if ($file =~ /\.\./)
			{
				$file =~ s/\.\.\///;
			}
			else
			{
				$file = "$basedir/$file";
			}

			$file =~ /(^\w+)/;
			my $dir = $vcfile;

			$dir =~ tr/[a-z]/[A-Z]/;
			$file = "../src/$file";
			if (! -f "$file" )
			{	
				print "Not found \"$dir - $file\"\n";
			}
			else
			{
				#$file =~ tr/[a-z]/[A-Z]/;
				$usedFiles{$file} = 1;
				#print "\"$file\"\n";
			}
		}
	}
	close(IN);
}

opendir(DIR, "../src") || die "ERROR: Opendir ..\\src";
my @dirs = grep { !/^\./ } readdir(DIR);
closedir(DIR);

my $dir;
foreach $dir (@dirs)
{
	#print "Checking dir ../src/$dir\n";
	checkDir("../src/$dir");
}

sub checkDir
{
	my ($dir) = @_;
	return if (! -d $dir);

	opendir(DIR, $dir) || die "ERROR: Cannot open \"$dir\"";
	my @files = grep { !/^\./ } readdir(DIR);
	closedir(DIR);

	my $file;
	foreach $file (@files)
	{
		my $wholeName = $dir."/".$file;
		#$wholeName =~ tr/[a-z]/[A-Z]/;

		next if (! -f $wholeName);

		if (! exists $usedFiles{$wholeName})
		{
			print "$wholeName\n";
		}
		else
		{
			#print "    Used - $wholeName\n";
		}
	}
}
