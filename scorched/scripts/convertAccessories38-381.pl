#!/usr/bin/perl -w

use strict;
use XML::DOM;
use XML::Parser::PerlSAX;

if (!defined $ARGV[0])
{
	print "Usage <accessories file>";
	exit 1;
}

my $file = $ARGV[0];
my $parser = XML::DOM::Parser->new();
my $newdoc = XML::DOM::Document->new;
my $root = $newdoc->createElement('accessories');
$newdoc->appendChild($root);

my $doc = $parser->parsefile($file);
foreach my $accessory ($doc->getElementsByTagName('accessory'))
{
	#print getContent($accessory);
	#print "\n";

	my $scorchedAccessory = $newdoc->createElement('accessory');
	$root->appendChild($scorchedAccessory);
	$accessory->setOwnerDocument($newdoc);

	moveChild($accessory, $scorchedAccessory, 'name');
	removeAll($accessory, 'name');
	moveChild($accessory, $scorchedAccessory, 'armslevel');
	removeAll($accessory, 'armslevel');
	moveChild($accessory, $scorchedAccessory, 'description');
	removeAll($accessory, 'description');
	moveChild($accessory, $scorchedAccessory, 'icon');
	removeAll($accessory, 'icon');
	moveChild($accessory, $scorchedAccessory, 'activationsound');
	removeAll($accessory, 'activationsound');
	moveChild($accessory, $scorchedAccessory, 'bundlesize');
	removeAll($accessory, 'bundlesize');
	moveChild($accessory, $scorchedAccessory, 'cost');
	removeAll($accessory, 'cost');
	moveChild($accessory, $scorchedAccessory, 'nonpurchasable');
	removeAll($accessory, 'nonpurchasable');
	moveChild($accessory, $scorchedAccessory, 'firedsound', 'activationsound');
	removeAll($accessory, 'firedsound');
	moveChild($accessory, $scorchedAccessory, 'deathanimationweight');
	removeAll($accessory, 'deathanimationweight');
	
	copyChild($accessory, $scorchedAccessory, 'projectilemodel', 'model');
	copyChild($accessory, $scorchedAccessory, 'projectilescale', 'modelscale');
	
	$accessory->setTagName('accessoryaction');
	$scorchedAccessory->appendChild($accessory);	
}

printNodes($newdoc, -1);

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
				print $kid->getNodeValue();
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
				print "\t";
			}
		
			print "<".$kid->getNodeName();
			
			if (defined $kid->getAttributes)
			{
				for (my $i=0; $kid->getAttributes->item($i); $i++)
				{
					print " ";
					print $kid->getAttributes->item($i)->getNodeName();
					print "='";
					print $kid->getAttributes->item($i)->getNodeValue();
					print "'";
				}
			}		
			
			print ">";
			print "\n" if ($foundChildren);
			printNodes($kid, $indent);
			
			if ($foundChildren)
			{
				for (my $i=0; $i<$indent; $i++)
				{
					print "\t";
				}
			}
			print "</".$kid->getNodeName().">";
			print "\n";
		}
	}
}

sub moveChild
{
	my ($src, $dest, $name, $newname) = @_;
    	my @values = $src->getElementsByTagName($name, 0);
	return if ($#values == -1);
	my $value = $values[0];
	return if (!defined $value);
	$src->removeChild($value);
	$dest->appendChild($value);
	$value->setTagName($newname) if (defined $newname);
}

sub copyChild
{
	my ($src, $dest, $name, $newname) = @_;
    	my @values = $src->getElementsByTagName($name, 0);
	return if ($#values == -1);
	my $value = $values[0];
	return if (!defined $value);
	my $newvalue = $value->cloneNode(1);
	
	$dest->appendChild($newvalue);
	$newvalue->setTagName($newname) if (defined $newname);
}

sub renameAll
{
	my ($src, $name, $newname) = @_;
	my @values = $src->getElementsByTagName($name, 1);
	foreach my $ele (@values)
	{
		$ele->setTagName($newname);
	}
}

sub removeAll
{
	my ($src, $name) = @_;
	my @values = $src->getElementsByTagName($name, 1);
	foreach my $ele (@values)
	{
		$ele->getParentNode->removeChild($ele);
	}
}

sub getContent
{
	my ($element) = @_;
	my @values = $element->getElementsByTagName('name');
	return undef if ($#values == -1);
	my $value = $values[0]->getFirstChild;
	return $value->getNodeValue if (defined $value);
	return "";
}
