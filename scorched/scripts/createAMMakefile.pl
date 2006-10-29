#!/usr/bin/perl

use strict;

sub getFiles
  {
    my ($vcfile) = @_;

open (IN, $vcfile) || die;
my @files = grep { /\.cpp\"/ or /\.c\"/ or /\.h\"/ } <IN>;
close (IN);
for (my $i=0; $i<=$#files; $i++)
  {
    chomp $files[$i];
    $files[$i] =~ s/^[^\"]*\"/\t/;
    $files[$i] =~ s/\".*$//;
    $files[$i] =~ s/\\/\//g;

    $files[$i] .= " \\" if ($i < $#files);
    $files[$i] .= "\n";
  }

    return @files;
}

sub locatefiles
{
	my ($dir, $basetypedir, $destdir) = @_;
	opendir(DIR, "../$dir") || die "ERROR: DIR ../$dir";
	my @files = grep { !/^\./ && !/CVS/ } readdir(DIR);
	closedir(DIR);

	my @newdirs = ();
	my @newfiles = ();
	my $file;
	foreach $file (@files)
	{
		if (-d "../$dir/$file") 
		{
			push @newdirs, "$file";
		}
		else
		{
			push @newfiles,"$dir/$file";
		}
	}

	my $newdir = $dir;
	$newdir =~ s!/!!g;
	$newdir =~ s!-!!g;
	print CLIENT "scorched${newdir}dir = $destdir\n";
	print CLIENT "scorched${newdir}_DATA = " . join(" \\\n\t", @newfiles) . "\n";
	foreach $file (@newdirs)
	{
		locatefiles("$dir/$file", $basetypedir, "$destdir/$file");
	}
}

open (CLIENT, ">../Makefile.am") || die "ERROR: Failed to write to ../Makefile.am";
print CLIENT "SUBDIRS = src\n\n";
print CLIENT "docdir = \@docdir\@\n\n";
locatefiles("documentation", "\$\{docdir\}", "\$\{docdir\}");
locatefiles("data", "\$\{datadir\}", "\$\{datadir\}/data");
close(CLIENT);

my @clientfiles = getFiles("../src/scorched/scorched.vcproj");

open (CLIENT, ">../src/scorched/Makefile.am") || die "../src/scorched/Makefile.am";

print CLIENT << "EOF";
## Process with automake to produce Makefile.in
                                                                                
bin_PROGRAMS = scorched3d

scorched3d_SOURCES = \\
EOF

print CLIENT @clientfiles;

print CLIENT << "EOF";

INCLUDES = -I../porting -I.. 
EOF
close(CLIENT);

