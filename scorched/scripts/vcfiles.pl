#!/tools/misc/bin/perl

my $dir = shift @ARGV;
if (!defined $dir)
{
	print "Usage : $0 <dir> <files...>\n";
	exit 1;
}

my @files = sort @ARGV;
foreach (@files)
{
print << "EOF";
			<File
				RelativePath="../$dir/$_">
			</File>
EOF
}
