
package master;

@ISA = qw(comp);

@FROM = (
    '%PRE%user:([^%]*)%',     '$1',         'user:$1',       'user:$1',    '$1'],
    '#PRE#user:([^%]*)#',     '$1',         'user:$1',       'user:$1',    '$1'],
    '%PRE%([^%]*)\$([^%]*)%', '$1\$$2',     '$2:$1',         '$2:$1',      '$1__M_$2'],
    '#PRE#([^%]*)\$([^%]*)#', '$1\$$2',     '$2:$1',         '$2:$1',      '$1__M_$2'],
    '%PRE%PROLOG%',           'Master',     'Sicstus',       'Yap',        'ilProlog'],
    'my_use_module',          'use_module', 'my_use_module', 'use_module', 'my_use_module'],
    'my_compile',             'consult',    'my_compile',    'my_compile', 'consult'],
);

@TO = (
);


sub new {
        my $type = shift;
        my $self = {};
        bless $self;
}

sub prolog_header {
        # always compatibility mode    
        print comp::PC ":- compatibility.\n";
}

sub process_specific {
    for ($k = 0; $k < @FROM; $k++) {
        $c = "\$main::line =~ s/$FROM[$k]/$TO[$k]/g";
        eval($c);
    }
}

sub second_pass {
    
# FIXME - does not work for now    
    
# for MasterProLog, a second pass is necessary to change :- module(name,list)
# into the correct format
# assumptions :-( : first line should start with ":- module(name, ["
#                   last line should end with " ] ) ."
#                   no comments etc. allowed in between
	unless (($name =~ /\.c$/)||($name =~ /\.C$/)
		||($name =~ /\.lpd$/)||($name =~ /\.h$/)) {    
	                           # only for Prolog files, not .c.gen and .lpd.gen files!
	    open(Master, "$specific_dir[0]/$name$suffix[0]"); 
	    open(Tempfile, "> $specific_dir[0]/$name$suffix[0].tmp"); 
	    while ($line = <Master>) {
		# scan for " :- module ( ... , [ " where ... contains no )
		# if found, continue until ] read
		if ($line =~ /^\s*:-\s*module\s*\([^\)]*,[^\[]*\[/) {
		    $emptylines = "";   # keep processed file the same length as .gen file
		    while (! ($line =~ /\]\s*\)\s*\./)) {
			$line .= <Master>; 
			$emptylines .= "\n" ;
		    }
		    $line =~ /^\s*:-\s*module\s*\(\s*([^,\s]*)\s*,\s*\[([^\]]*)\]/; 
#	$line = create_globals $1, $2; # convert module decl for MasterProLog
		    print_imports $1, $2;
		    $line = ":- module($1).\n".$emptylines;
		}
		elsif ($line =~ /^\s*:-\s*use_module\s*\(\s*([^\)\s\,]*)\s*\)/) {
		    $mod = $1;
		    $line = usemod1 $mod;
		}
		elsif ($line =~ /^\s*:-\s*use_module\s*\([^\)]*,[^\[]*\[/) {
		    while (! ($line =~ /\]\s*\)\s*\./)) { $line .= <Master>; }
		    $line =~ /^\s*:-\s*use_module\s*\(\s*([^,\s]*)\s*,\s*\[([^\]]*)\]/;
		    $mod = $1;
		    $list = $2;
		    $line = usemod2 $mod, $list;
		}
		print Tempfile $line;
	    }
	    close Tempfile;
	    close Master;
	    system("mv $specific_dir[0]/$name$suffix[0].tmp".
		   " $specific_dir[0]/$name$suffix[0] 2>/dev/null");
	} # end unless    
}

sub get_gapic_suffix {
    return ".ilp";
}

return 1;
