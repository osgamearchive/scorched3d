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

my @clientfiles = getFiles("../src/scorched/scorched.vcproj");

open (CLIENT, ">../src/scorched/Makefile.am") || die;

print CLIENT << "EOF";
## Process with automake to produce Makefile.in
                                                                                
EXTRA_DIST = scorched.vcproj

noinst_PROGRAMS = scorched3d

scorched3d_SOURCES = \\
EOF

print CLIENT @clientfiles;

print CLIENT << "EOF";

INCLUDES = -I../porting -I.. 
EOF

