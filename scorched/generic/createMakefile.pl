#!/usr/bin/perl

use strict;

#Print makefile header
open(OUT, ">Makefile") || die "ERROR:out";
print OUT "# General definitions\n";
print OUT "RM = rm -f\n";
print OUT "CC = /opt/gcc-3.3.2-bin/bin/gcc\n";
print OUT "CPP = /opt/gcc-3.3.2-bin/bin/g++\n";
print OUT "DEFINES = -DNO_FLOAT_MATH -DdDOUBLE\n";
print OUT "DEFINESscorched = \$(DEFINES) -D_FILE_OFFSET_BITS=64 -D_LARGE_FILES -D__WXGTK__ -D_REENTRANT -DGTK_NO_CHECK_CASTS\n";
print OUT "INCLUDE = -I../src -I/opt/gcc-3.3.2-bin/include/c++/3.3.2/ -I../src/porting -I/opt/wxGTK-2.4.1/lib/wx/include/gtk-2.4 -I/opt/wxGTK-2.4.1/include -I/usr/openwin/include -I/opt/SDL/include -I/usr/cisco/packages/gtk+/gtk+-1.2.10/include/gtk-1.2 -I/usr/cisco/packages/glib/glib-1.2.10/include/glib-1.2\n";
print OUT "CCFLAGS = \$(INCLUDE) -g -O2 -c -pthreads\n";
print OUT "LINKFLAGS = -g -O2 -v \n";
print OUT "LINKLIBS = -pthreads -lGL -lGLU -lSDL -lXm -lXpm -lXmu -lXext -lXt -lSM -lICE -lX11 -ljpeg -lz -lposix4 -lnsl -lsocket -ldl -lm -lSDL_mixer -lSDL_net -lpthread -L/opt/gcc-3.3.2-bin/lib/ -L/lib/X11 -R/lib/X11 -L/opt/SDL/lib -R/opt/SDL/lib -L/opt/wxGTK-2.4.1/lib/ -lwx_gtk-2.4 -L/usr/cisco/packages/gtk+/gtk+-1.2.10/lib -L/usr/cisco/packages/glib/glib-1.2.10/lib -lglib -lgthread -lgtk -lgdk\n";
print OUT "\n";

my %files = ();
my %alldeps = ();
my %nofiles = ();

print OUT "all : \\\n";
print OUT "\tscorched\n\n";

print OUT "scorched : \\\n";
print OUT "\t../src/scorched/Release/scorchedg\n\n";

print OUT "clean : \\\n";
print OUT "\tcleanscorched \n\n";

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
			if ($file =~ /\.cpp$/ or $file =~ /\.c$/)
			{
				if ($file =~ /\.\./)
				{
					$file =~ s/\.\.\\//;
				}
				else
				{
					$file = "$basedir//$file";
				}
				$file =~ s/\\/\//g;
				$file =~ /(^\w+)/;
				my $dir = $1;
				$dir =~ tr/[a-z]/[A-Z]/;
				$file = "../src/$file";

				${$files{$dir}}{$file} = 1;
				push @{$localfiles{$dir}}, $file;
			}
		}
	}
	close(IN);

	print OUT "# Libraries for $exename\n";
	my ($key, $value);
	while (($key, $value) = each(%localfiles))
	{
		print OUT uc($exename).$key."OBJECTS = \\\n";
		my $maxi = $#{$value};
		for (my $i=0; $i<=$maxi; $i++)
		{
			my $file = ${$value}[$i];

			my $obj = $file;
			$obj =~ s/\.cpp$/$exename\.o/;
			$obj =~ s/\.c$/$exename\.o/;
			$obj =~ s/\///g;
			$obj =~ s/\\//g;
			$obj =~ s/\.\.//g;
			$obj = "../src/$basedir/Release/$obj";

			print OUT "\t".$obj;
			print OUT " \\" if ($i != $maxi);
			print OUT "\n";
		}
		print OUT "\n";
	
	}

	print OUT "\n../src/scorched/Release/".$exename."g : \\\n";
	my @list = keys %localfiles;
	for (my $i=0; $i<=$#list; $i++)
	{
		$_ = uc($exename).$list[$i];
		print OUT "\t\$(".$_."OBJECTS) ";
		print OUT "\\" if ($i != $#list);
		print OUT "\n";
	}
	print OUT "\t\$(CPP) \$(LINKFLAGS) \$(LINKLIBS)\\\n";
	for (my $i=0; $i<=$#list; $i++)
	{
		$_ = uc($exename).$list[$i];
		print OUT "\t\$(".$_."OBJECTS) \\\n";
	}
	print OUT "\t-o ../src/scorched/Release/".$exename."g\n\n";

	print OUT "clean".$exename." :\n";
	print OUT "\t\$(RM) ../src/scorched/Release/".$exename."g\n";

	foreach (keys %localfiles)
	{
		$_ = uc($exename).$_;
		print OUT "\t\$(RM) \$(".$_."OBJECTS)\n";
	}
	print OUT "\n";

	print OUT "# CPP Build Rules\n";
	while (($key, $value) = each(%localfiles))
	{
		my $maxi = $#{$value};
		for (my $i=0; $i<=$maxi; $i++)
		{
			my $file = ${$value}[$i];

			print $file."\n";
			my @filedeps = getAllDeps($file);		

			my $obj = $file;
			$obj =~ s/\.cpp$/$exename\.o/;
			$obj =~ s/\.c$/$exename\.o/;
			$obj =~ s/\///g;
			$obj =~ s/\\//g;
			$obj =~ s/\.\.//g;
			$obj = "../src/$basedir/Release/$obj";

			push @{$alldeps{$obj}}, @filedeps if ($#filedeps > -1);
			print OUT "$obj : $file\n";
			print OUT "\t\$(CC) \$(CCFLAGS) \$(DEFINES$exename) -c -o $obj $file\n\n";
		}
	}
}

print OUT "# Other Deps\n";
my ($key, $value);
while (($key, $value) = each(%alldeps))
{
	print OUT "$key:";
	foreach (@{$value})
	{
		print OUT " ".$_;
	}	
	print OUT "\n";
}
close(OUT);

print "Cannot find\n";
print join("\n", sort keys %nofiles);

sub getAllDeps
{
	my ($file) = @_;

	my %deps = ();
	getDeps($file, \%deps);
	return keys %deps;
}

sub getDeps
{
	my ($file, $deps) = @_;

	return if (defined ${$deps}{$file});
	return if (defined $nofiles{$file});

	if (!open (IN, "$file"))
	{
		$nofiles{$file} = 1;
		return;	
	}
	my @filelines = <IN>;
	close (IN);

	${$deps}{$file} = 1;
	my $line;
	foreach $line (@filelines)
	{
		if ($line =~ /\#include\s+[\<\"]([^\>]+)[\>\"]/)
		{
			my $match = $1;
			if ($line =~ /\"/)
			{
				getDeps("../src/scorched/".$match, $deps);
			}
			else
			{
				getDeps("../src/".$match, $deps);
			}
		}
	}
}
