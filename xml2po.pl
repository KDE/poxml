#!/usr/bin/perl -w
#
# extracts text parts from a xml file to a po file to
# make them translatable with advanced tools
#
# usage:
#   doc2po.pl [-v] [-xhtml|-docbook] <xml-file> <po-file>
#
################# komenco de la programo ################
 
use XML::Parser;

$|=1;
$max_line_len =79;
 
# read options
$format = 'docbook'; #default

while ($ARGV[0] =~ /^\-/) {
    if ($ARGV[0] eq '-h' or $ARGV[0] eq '--help') {
	helpscreen();
	exit;
    } elsif ($ARGV[0] eq '-v') {
        $verbose = 1;
        shift @ARGV;
    } elsif ($ARGV[0] eq '-xhtml' or $ARGV[0] eq '-docbook') {
	$format = shift @ARGV;
    } else {
	helpscreen();
	die "Invalid option \"$ARGV[0]\"!\n";
    }
};

# next to args should be the files
$xmlfile = shift @ARGV;
$pofile = shift @ARGV;

unless ($xmlfile and $pofile and not @ARGV) {
    helpscreen();
    die "Invalid number of arguments.\n";
}

# check if xmlfile exist
die "XML file \"$xmlfile\" doesn't exist!\n" unless (-f $xmlfile);
 
# now parse the xml file and extract its contents to
# the hash %messages

parse_xml();

# finally output the messages in @messages to the po file
output_po();

                   ### end of main ###

###################### xml parsing ##################

sub parse_xml {
    my $parser;
    local $message = '';
    local $inside = 0;

    if ($format eq 'docbook') {
	$parser = new XML::Parser(#ParseParamEnt => 1,
				  NoExpand => 1,
				  ErrorContext => 2,
				  Handlers => {
				      Start => \&start_handler_docbook,
				      End   => \&end_handler_docbook,
				      Default  => \&default_handler_docbook,
				  }
				  );
    } else {
	die "This file format is not suported yet\n";
    }


    print "parsing \"$xmlfile\"...\n" if ($verbose);

    eval { $parser->parsefile($xmlfile) }; warn $@ if ($@);   
}

sub default_handler_docbook {
    my ($xp, $text) = @_;
 
    if  ($inside)
    {
        $message .= $xp->recognized_string;
    }
}

sub start_handler_docbook {
    my ($xp,$el,@attrs) = @_;
    my $attr;

    print "." if ($verbose);
 
    $current_line = $xp->current_line();

    if ($inside) {
	$message .= $xp->recognized_string();
    };

    if (
        $el eq 'title' or
        $el eq 'para'
        )
    {
	$message = '' unless ($inside);
	$inside++;
    }
}

sub end_handler_docbook {
    my ($xp, $el) = @_;
 
    if (
        $el eq 'title' or
        $el eq 'para'
        )
    {
	$inside--;
        unless ($inside) {
	    if ($messages{$message}) {
		$messages{$message} .= ",$current_line";
	    } else {
		$messages{$message} = $current_line;
	    }
	}
    } elsif ($inside) {
	$message .= $xp->recognized_string();
    }
}

######################## po file output ##############

sub output_po {
    open OUT, ">$pofile" or
	die "Couldn't open \"$pofile\" for writing: $!\n";
    select OUT;

    po_header();
    po_messages();

    select STDOUT;
    close OUT;
}

sub po_header {
    print <<EOH;
# SOME DESCRIPTIVE TITLE.
# Copyright (C) YEAR Free Software Foundation, Inc.
# FIRST AUTHOR <EMAIL\@ADDRESS>, YEAR.
#
#, fuzzy
msgid ""
msgstr ""
"Project-Id-Version: PACKAGE VERSION\\n"
"POT-Creation-Date: 2001-02-09 01:25+0100\\n"
"PO-Revision-Date: YEAR-MO-DA HO:MI+ZONE\\n"
"Last-Translator: FULL NAME <EMAIL\@ADDRESS>\\n"
"Language-Team: LANGUAGE <LL\@li.org>\\n"
"MIME-Version: 1.0\\n"
"Content-Type: text/plain; charset=CHARSET\\n"
"Content-Transfer-Encoding: ENCODING\\n"

EOH
}

sub po_messages {

    while (($msg,$pos) = each %messages) {

	print "#: $xmlfile:$pos\n";
	my @m = format_message($msg);
	if ($#m == 0) {
	    print "msgid \"$m[0]\"\n";
	} else {
	    print "msgid \"\"\n";
	    foreach (@m) {
		print "\"$_\"\n";
	    }
	}
	print "msgstr \"\"\n\n";
    }
}

sub format_message {
    my $msg = shift;
    my @res = ();

    # escapes
    @msg = split("\n",$msg);
    foreach $m (@msg) {
          while (length($m) > $max_line_len) {
               $x = (substr($m,0,$max_line_len));
               $m = substr($m,$max_line_len);

	       $x =~ s/([\t"])/\\$1/sg;
               $x =~ s/\n/\\n\n/sg;
               push @res,$x;
          }

          $m =~ s/([\t"])/\\$1/sg;
          $m =~ s/\n/\\n\n/sg;
          push @res,$m;
    }
 
    return @res;
}

####################### helpscreen ##################

sub helpscreen {
    print <<EOM;

    xml2po.pl (c) 2001 by Wolfram Diestel <wolfram\@steloj.de>
              (license: GPL 2.0 or later)

    Parses a xml file extracting some element content and writes
    the extracted text into a po template file for translation.
   
    usage:
      doc2po.pl [-v] [-xhtml|-docbook] <xml-file> <pot-file>

    If no format is given docbook is used. The xml file should
    be wellformed. The format says what elements are extracted.
    Elements, which normally has no content for translation
    (author, copyright...) are ignored.

EOM
}
