#!/usr/local/gnu/bin/perl

if (($#ARGV != 2) && ($#ARGV != 3)) {
	print "Usage: mrep [FILE] [SEARCH] [NEWLINE]\n";
	exit(-1);
}

open(IN, "$ARGV[0]") || die "Can't open $ARGV[0]\n";
open(OUT, ">$ARGV[0].tmp") || die "Can't open $ARGV[0].tmp\n";

if ($#ARGV == 3) {
    $fct = "\$line =~ s/$ARGV[2]/$ARGV[3]/g";
    while ($line = <IN>) {
        chop($line);
	    eval($fct);
        print OUT "$line\n";	    
    }    
} else {
    $fct = "\$line =~ /$ARGV[1]/";
    while ($line = <IN>) {
	    chop($line);
	    if (eval($fct)) {
    		print OUT "$ARGV[2]\n";
    	} else {
		    print OUT "$line\n";
	    }	
    }
}

close(OUT);
close(IN);

system("cp $ARGV[0].tmp $ARGV[0] 2>/dev/null");
system("rm $ARGV[0].tmp 2>/dev/null");
