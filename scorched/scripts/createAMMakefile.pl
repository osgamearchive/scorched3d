#!/usr/bin/perl

use strict;

sub getFiles
  {
    my ($vcfile) = @_;

open (IN, $vcfile) || die $vcfile;
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

sub createBinaryMakefile
{
my ($input, $output, $binary, $flags) = @_;

my @clientfiles = getFiles($input);

open (CLIENT, ">$output") || die $output;

print CLIENT "bin_PROGRAMS = $binary\n\n";
print CLIENT $binary."_SOURCES = \\\n";
print CLIENT @clientfiles;
print CLIENT "\n\nAM_CPPFLAGS = -I../porting -I.. ${flags}\n\n";

close(CLIENT);

}

sub createInstallMakefile
{
open (CLIENT, ">../Makefile.am") || die "ERROR: Failed to write to ../Makefile.am";
print CLIENT "SUBDIRS = src\n\n";
print CLIENT "docdir = \@docdir\@\n\n";
locatefiles("documentation", "\$\{docdir\}", "\$\{docdir\}");
locatefiles("data", "\$\{datadir\}", "\$\{datadir\}/data");
close(CLIENT);
}

createInstallMakefile();
createBinaryMakefile(
	"../src/scorched/scorched.vcproj", 
	"../src/scorched/Makefile.am", 
	"scorched3d",
	"");
createBinaryMakefile(
	"../src/scorchedc/scorchedc.vcproj", 
	"../src/scorchedc/Makefile.am", 
	"scorched3dc",
	"");
createBinaryMakefile(
	"../src/scorcheds/scorcheds.vcproj", 
	"../src/scorcheds/Makefile.am", 
	"scorched3ds",
	"-DS3D_SERVER=1");

