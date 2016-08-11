
package sicstus;

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
       
sub process_specific {
    for ($k = 0; $k < @FROM; $k++) {
        $c = "\$main::lineline =~ s/$FROM[$k]/$TO[$k]/g";
        eval($c);
    }
}       
       
sub get_gapic_suffix {
    return ".pl";
}       

sub gapic_print_header {
    print comp::GGLUE "foreign(gapic_module_init, gapic_init_$main::module_id).\n";
}    

sub gapic_footer {
	print comp::GGLUE "foreign_resource(\'$main::c_module_name\',".
		              "[$main::gapic_list]).\n";
	print comp::GGLUE ":- load_foreign_resource(\'$main::c_module_name\').\n";
	print comp::GGLUE ":- gapic_init_$main::module_id.\n";
}

sub gapic_begin {    
    print comp::GGLUE "gapic_foreign(\'\$SRC_DIR/$main::choppedname\',$main::gapic_predname).\n";
    print comp::GGLUE "foreign($main::gapic_predname,$main::gapic_predname";    
    print comp::PC "int $main::gapic_predname(\n";
    if ($gapic_argcnt > 0) {
        print comp::GGLUE "(";
    }	
    for ($k=0; $k < $main::gapic_argcnt; $k++) {
        $argmode = $main::gapic_argmode[$k];
        $argtype = $main::gapic_argtype[$k];
        $argname = $main::gapic_argname[$k];
        if($argtype eq "term") {
 	    print comp::PC "\tGAPIC_term_ref "; 
	} else {
 	    print comp::PC "\tGAPIC_$argtype ";
	}
	if($argmode eq "out") {
	    print comp::PC "*";
        print comp:GGLUE "-";
	} else {
        print comp:GGLUE "+";
	}
	print comp:PC $argname;
	if($argtype eq "int") {
        print comp:GGLUE "integer";
    } elsif($argtype eq "float") {
        print comp:GGLUE "float";
    } elsif($argtype eq "atom") {
        print comp:GGLUE "atom";
    } elsif($argtype eq "term") {
        print comp:GGLUE "term";
    } if($k < $main::gapic_argcnt-1) {
        print comp:PC ",\n";
        print comp:GGLUE ",";
    }
    print comp:PC ") {\n";
    if($main::gapic_argcnt > 0) {
            printf comp:GGLUE ")";
    }	
    print comp:GGLUE ").\n";    
}

sub gapic_commit {
    print comp::PC "}\n";    
}
       
return 1;
