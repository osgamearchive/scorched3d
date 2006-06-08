use strict;
use XML::DOM;
use XML::Parser::PerlSAX;

if ($#ARGV < 0)
{
	print "Usage : $0 <files ...>\n\tFiles to be converted.";
	exit 1;
}

foreach my $infile (@ARGV)
{
	processFile($infile);
}

sub processFile
{

my ($file) = @_;

my $parser = XML::DOM::Parser->new();
my $doc = $parser->parsefile($file);
my $found = 0;

foreach my $placement ($doc->getElementsByTagName('placement'))
{
	my @eles = $placement->getElementsByTagName('groupname');
	if ($#eles >= 0)
	{
		$found = 1;
		my $parent = $eles[0]->getParentNode();
		my $node = $parent->removeChild($eles[0]);

		foreach my $object ($placement->getElementsByTagName('object'))
		{
			my $type = $object->getAttributeNode("type")->getValue();
			if ($type eq "tree" || $type eq "target" || $type eq "model")
			{
				$object->appendChild($node->cloneNode(1));
			}
		}
	}
}

if ($found)
{
	open(OUT, ">".$file);
	printNodes($doc, -1);
	close(OUT);
}
}

sub printNodes
{
	my ($node, $indent) = @_;
	$indent++;
	for my $kid ($node->getChildNodes)
	{
		if ($kid->getNodeName() eq "#text")
		{
			if ($kid->getNodeValue() !~ /^\s*$/)
			{
				print OUT $kid->getNodeValue();
			}
		}
		else
		{
			my $foundChildren = 0;
			for my $secondkid ($kid->getChildNodes)
			{
				if ($secondkid->getNodeName() ne "#text")
				{
					$foundChildren = 1;
				}
			}
			for (my $i=0; $i<$indent; $i++)
			{
				print OUT "\t";
			}
		
			print OUT "<".$kid->getNodeName();
			
			if (defined $kid->getAttributes)
			{
				for (my $i=0; $kid->getAttributes->item($i); $i++)
				{
					print OUT " ";
					print OUT $kid->getAttributes->item($i)->getNodeName();
					print OUT "='";
					print OUT $kid->getAttributes->item($i)->getNodeValue();
					print OUT "'";
				}
			}		
			
			print OUT ">";
			print OUT "\n" if ($foundChildren);
			printNodes($kid, $indent);
			
			if ($foundChildren)
			{
				for (my $i=0; $i<$indent; $i++)
				{
					print OUT "\t";
				}
			}
			print OUT "</".$kid->getNodeName().">";
			print OUT "\n";
		}
	}
}