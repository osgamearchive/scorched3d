use strict;

#Print makefile header
open(OUT, ">Makefile") || die "ERROR:out";
print OUT "# Change these two lines to point to the location borland compiler\n";
print OUT "# and the dx sdk\n";
print OUT "BORLANDDIR = L:\\Borland\\BCC55\n";
print OUT "\n\n";
print OUT "# General definitions\n";
print OUT "RM = del\n";
print OUT "TLIB = \$(BORLANDDIR)\\Bin\\tlib.exe\n";
print OUT "ILINK = \$(BORLANDDIR)\\Bin\\ilink32.exe\n";
print OUT "BRC32 = \$(BORLANDDIR)\\Bin\\brc32.exe\n";
print OUT "TOUCH = \$(BORLANDDIR)\\Bin\\touch.exe\n";
print oUT "PATH = $(BORLANDDIR)\\Bin\n";
print OUT "LINKLIBS = import32.lib + cw32mt.lib + dxguid.lib + sdl.lib + sdlmain.lib + SDL_mixer.lib + SDL_net.lib + wx32d.lib\n";
print OUT "CC = \$(BORLANDDIR)\\Bin\\bcc32.exe\n";
print OUT "INCLUDE = -I..\\src -I\$(BORLANDDIR)\\Include\n";
print OUT "LIBS = -L\$(BORLANDDIR)\\lib -L\$(BORLANDDIR)\\dxlib -L\$(BORLANDDIR)\\sdllib -L\$(BORLANDDIR)\\wxlib\n";
print OUT "DEFINES = -D__WXWIN__ -D__WXMSW__ -D__WINDOWS__ -DWIN32 -DWXDEBUG=1 -D__WXDEBUG__ -DUSE_DEFINE -D__WIN95__ -D__WINDOWS__ -DNO_FLOAT_MATH -DCOMPILED_FROM_DSP -D_NO_SERVER_ASE_ -D__MSC;INC_OLE2;__WIN95__;__WINDOWS__;WIN32;__BIDE__;dDOUBLE\n";
print OUT "DEFINESscorchedsvr = \$(DEFINES) -DSCORCHED_SERVER\n";
print OUT "DEFINESscorched = \$(DEFINES)\n";
print OUT "CCFLAGS = \$(INCLUDE) -Od -a8 -5 -tWM -W -w -w! -w-inl -w-8022 -w-8008 -w-8071 -w-8080 -w-8084 -w-8017 -w-8001 -w-8060 -w-rch -w-aus -w-par -v \n";
print OUT "LINKFLAGS = -I..\\release -x -Gn -Tpe -aa -c -v -V4.0\n";
print OUT "\n";

my %alldeps = ();
my %nofiles = ();

print OUT "all : \\\n";
print OUT "\tclient\n";

print OUT "client : \\\n";
print OUT "\t..\\src\\scorched\\Release\\scorchedb.exe\n\n";

print OUT "clean : \\\n";
print OUT "\tcleanscorched \n";

addVCProj("..\\src\\scorched\\scorched.vcproj", "scorched", "scorched");

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
					$file = "$basedir\\$file";
				}

				$file =~ /(^\w+)/;
				my $dir = $1;
				$dir =~ tr/[a-z]/[A-Z]/;
				$file = "..\\src\\$file";

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
			$obj =~ s/\.cpp$/$exename\.obj/;
			$obj =~ s/\.c$/$exename\.obj/;
			$obj =~ s/\///g;
			$obj =~ s/\\//g;
			$obj =~ s/\.\.//g;
			$obj = "..\\src\\$basedir\\release\\$obj";

			print OUT "\t".$obj;
			print OUT " \\" if ($i != $maxi);
			print OUT "\n";
		}
		print OUT "\n";
	
	}

	print OUT "\n..\\src\\scorched\\release\\".$exename."b.exe : \\\n";
	my @list = keys %localfiles;
	for (my $i=0; $i<=$#list; $i++)
	{
		$_ = uc($exename).$list[$i];
		print OUT "\t..\\src\\scorched\\release\\".$_.".lib ";
		print OUT "\\ " if ($i != $#list);
		print OUT "\n";
	}
	print OUT "\t\$(ILINK) \$(LIBS) \$(LINKFLAGS) c0w32.obj, ..\\src\\scorched\\release\\".$exename."b.exe,, \\\n";
	print OUT "\t\$(LINKLIBS) + \\\n";
	for (my $i=0; $i<=$#list; $i++)
	{
		$_ = uc($exename).$list[$i];
		print OUT "\t..\\src\\scorched\\release\\".$_.".lib ";
		print OUT "+ " if ($i != $#list);
		print OUT "\\\n";

	}
	print OUT "\t, ,\n\n";
#..\\src\\$basedir\\release\\resource.res\n\n";

	foreach (@list)
	{
		$_ = uc($exename).$_;
		print OUT "..\\src\\scorched\\release\\".$_.".lib : \$(".$_."OBJECTS)\n";
		print OUT "\t\$(TOUCH) ..\\src\\scorched\\release\\".$_.".lib\n";
		print OUT "\t\$(RM) ..\\src\\scorched\\release\\".$_.".lib\n";
		print OUT "\t\$(TLIB) ..\\src\\scorched\\release\\".$_.".lib /a \$(".$_."OBJECTS) \n\n";
	}

	print OUT "clean".$exename." :\n";
	print OUT "\t\$(TOUCH) ..\\src\\scorched\\release\\".$exename."b.exe\n";
	print OUT "\t\$(RM) ..\\src\\scorched\\release\\".$exename."b.*\n";

	foreach (keys %localfiles)
	{
		$_ = uc($exename).$_;
		print OUT "\t\$(TOUCH) \$(".$_."OBJECTS)\n";
		print OUT "\t\$(RM) \$(".$_."OBJECTS)\n";
		print OUT "\t\$(TOUCH) ..\\src\\scorched\\release\\".$_.".lib\n";
		print OUT "\t\$(RM) ..\\src\\scorched\\release\\".$_.".lib\n";
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
			$obj =~ s/\.cpp$/$exename\.obj/;
			$obj =~ s/\.c$/$exename\.obj/;
			$obj =~ s/\///g;
			$obj =~ s/\\//g;
			$obj =~ s/\.\.//g;
			$obj = "..\\src\\$basedir\\release\\$obj";

			push @{$alldeps{$obj}}, @filedeps if ($#filedeps > -1);
			print OUT "$obj : $file\n";
			print OUT "\t\$(CC) \$(CCFLAGS) \$(DEFINES$exename) -c -o$obj $file\n\n";
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

	if (!open (IN, "..\\src\\$file"))
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
		if ($line =~ /\#include\s+\<([^\>]+)\>/)
		{
			getDeps("..\\src\\".$1, $deps);
		}
	}
}
