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
# defaults
$debug = 1;
$verbose = 0;
$format = 'docbook';

# read options
while ($ARGV[0] =~ /^\-/) {
    if ($ARGV[0] eq '-h' or $ARGV[0] eq '--help') {
	helpscreen();
	exit;
    } elsif ($ARGV[0] eq '-v') {
        $verbose = 1;
        shift @ARGV;
    } elsif ($ARGV[0] eq '-xhtml' or $ARGV[0] eq '-docbook') {
	$format = shift @ARGV;
    } elsif ($ARGV[0] eq '-o') {
	shift @ARGV;
	$outputfile = shift @ARGV;
    } else {
	helpscreen();
	die "Invalid option \"$ARGV[0]\"!\n";
    }
};

# next two args should be the input files
$xmlfile = shift @ARGV;
$pofile = shift @ARGV;

unless ($xmlfile and $pofile and not @ARGV) {
    helpscreen();
    die "Invalid number of arguments.\n";
}

# check if xmlfile exist
die "XML file \"$xmlfile\" doesn't exist!\n" unless (-f $xmlfile);
 

# finally output the messages in @messages to the po file
read_po();

if ($outputfile) {
    open OUT,">$outputfile" or 
	die "Couldn't open \"$outputfile\" for writing: $!\n";
    select OUT;
}
    
# now parse the xml file replacing it's content and output the result
parse_xml();

if ($outputfile) {
    select STDOUT;
    close OUT;
}

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

    warn "parsing \"$xmlfile\"...\n" if ($verbose);

    eval { $parser->parsefile($xmlfile) }; warn $@ if ($@);   
}

sub default_handler_docbook {
    my ($xp, $text) = @_;
 
    if  ($inside)
    {
        $message .= $xp->recognized_string();
    } else {
	print $xp->recognized_string();
    }
}

sub start_handler_docbook {
    my ($xp,$el,@attrs) = @_;
    my $attr;

    warn "." if ($verbose);
 
    #$current_line = $xp->current_line();

    if ($inside) {
	$message .= $xp->recognized_string();
    } else {
	print $xp->recognized_string();
    }

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

	# leaving element print the translated message
	unless ($inside) {
	    if ($messages{$message}) {
		print $messages{$message};
		warn "translated\n" if ($debug);
	    } else {
		print $message;
		warn "not translated\n" if ($debug);
	    } 
	}
    }

    if ($inside) {
	$message .= $xp->recognized_string();
    } else {
	print $xp->recognized_string();
    }
}


######################## po file output ##############

sub read_po {
    my ($msgid, $msgstr);

    open PO, $pofile or 
	die "Couldn't open \"$pofile\" for reading: $!\n";
    @lines = <PO>;
    close PO;

    for ($line = 0; $line < $#lines; $line++) {
	$_ = $lines[$line];

	next if (/^\#/); # ignore comments
	next if (/^\s*$/); # ignore empty lines

	if (/^msgid\s+\"(.+)\"\s*$/) { 
	    $msgid = $1;
	} elsif (/^msgid\s+\"\"\s*$/) {
	    # read all of msgid
	    $msgid = '';
	    $_ = $lines[++$line];
	    while (/^\"/) {
		s/^\"//; s/\"$//;
		# todo: replace some escape codes like \\ \" \n
		$msgid .= $_;
		$_ = $lines[++$line];
	    }
	    # go back one line
	    $line--;
	} elsif (/^msgstr\s+\"(.+)\"\s*$/) { 
	    $msgstr = $1;
	    # add message to hash
	    #print "MSGID: [$msgid]\n" . "MSGSTR: [$msgstr]\n\n" if ($debug);
	    $messages{unformat_message($msgid)} = unformat_message($msgstr);
	} elsif (/^msgstr\s+\"\"\s*$/) {
	    # read all of msgstr
	    $msgstr = '';
	    $_ = $lines[++$line];
	    while (/^\"/) {
		s/^\"//; s/\"$//;
		# todo: replace some escape codes like \\ \" \n
		$msgstr .= $_;
		$_ = $lines[++$line];
	    }
	    # go back one line
	    $line--;

	    # add message to hash
	    #print "MSGID: [$msgid]\n" . "MSGSTR: [$msgstr]\n\n" if ($debug);

	    $messages{unformat_message($msgid)} = unformat_message($msgstr);
	} else {
	    warn "Invalid line in po file at: $.\n";
	}
    }
}

sub unformat_message {
    my $msg = shift;

    # escapes
    $msg =~ s/\\n/\n/sg;
    $msg =~ s/\\"/"/sg;
    $msg =~ s/\\t/\t/sg;
 
    return $msg;
}


####################### helpscreen ##################

sub helpscreen {
    print <<EOM;

    po2xml.pl (c) 2001 by Wolfram Diestel <wolfram\@steloj.de>
              (license: GPL 2.0 or later)

    Merges a xml file with a po message file creating a
    translated xml file.
   
    usage:
      doc2po.pl [-v] [-xhtml|-docbook] [-o <new-xml-file>] <xml-file> <po-file> 
    If no format is given docbook is used. The xml file should
    be wellformed. The format says the content of what elements are replaced
    by the translations from de po-file. If a translation for 
    a content isn't find in the po-file the original (english) text is
    left.

EOM
}
