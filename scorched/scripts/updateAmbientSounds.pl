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

foreach my $placement ($doc->getElementsByTagName('ambientsound'))
{
	my @eles = $placement->getElementsByTagName('sound');
	foreach my $sound (@eles)
	{
		my @children = $sound->getChildNodes();
		if ($#children <= 1)
		{
			$found = 1;
			my $file = $children[0]->getNodeValue();
			my $parent = $children[0]->getParentNode();
			
			$parent->removeChild($children[0]);
			$parent->setAttribute("type", "file");
			
			my $fileele = $doc->createElement("file");
			$fileele->appendChild($doc->createTextNode($file));
			$parent->appendChild($fileele);

			my $gainele = $doc->createElement("gain");
			$gainele->appendChild($doc->createTextNode("1.0"));
			$parent->appendChild($gainele);			
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