
package ilp;

@ISA = qw(comp);

@FROM = (
    '%PRE%user:([^%]*)%',
    '#PRE#user:([^%]*)#',
    '%PRE%([^%]*)\$([^%]*)%',
    '#PRE#([^%]*)\$([^%]*)#',
    '%PRE%PROLOG%',
    'my_compile'
);

@TO = (
    '$1',
    '$1',
    '$1__M_$2',
    '$1__M_$2',
    'ilProlog',
    'consult'
);

sub new {
        my $type = shift;
        my $self = {};
        bless $self;
}

sub process_specific {
    for ($k = 0; $k < @FROM; $k++) {
        $c = "\$main::line =~ s/$FROM[$k]/$TO[$k]/g";
        eval($c);
    }
}

sub get_gapic_suffix {
    return ".ilp";
}

sub gapic_print_header {
    if ($main::aceos eq "Windows") {
        print comp::GGLUE ":- (('\$dlopen'('$main::c_module_name.dll',Ref),\n";
    } elsif ($main::aceos eq "Solaris") {
        print comp::GGLUE ":- (('\$dlopen'('lib$main::c_module_name.so',Ref),\n";    
    } else {
        print comp::GGLUE ":- (getenv('GAPIC_STATIC',_) -> true; (('\$dlopen'('$main::c_module_name.so',Ref),\n";    
    }
    print comp::GGLUE "\t'\$dlsym'('gapic_init_$main::module_id'/0,gapic_module_init_$main::module_id,Ref),\n";
}

sub gapic_footer {
    if ($main::aceos eq "Windows") {
	    print comp::GGLUE "\ttrue) -> true; gapic_error_load_so('$main::c_module_name.dll')).\n";
    } else {
	    print comp::GGLUE "\ttrue) -> true; gapic_error_load_so('$main::c_module_name.so'))).\n";
    }
    print comp::GGLUE ":- 'gapic_init_$main::module_id'.\n";
}

sub gapic_begin {
    print comp::GGLUE "\t\'\$dlsym\'($main::gapic_predname/$main::gapic_argcnt,$main::gapic_predname,Ref),\n";
    print comp::PC "SOPROVIDE boolean $main::gapic_predname(ILP_engine *machine) {\n";    
    for ($k=0; $k < $main::gapic_argcnt; $k++) {
        $argmode = $main::gapic_argmode[$k];
        $argtype = $main::gapic_argtype[$k];
        $argname = $main::gapic_argname[$k];
        if ($argtype eq "term") {
	        print comp::PC "\tGAPIC_term_ref $argname=".
                           "ILP_term_get_arg(machine,$k+1);\n";
        } elsif (($argtype eq "int") || 
                 ($argtype eq "float") || 
                 ($argtype eq "atom")) { 
            if($argmode eq "out") {
                print comp::PC "\tGAPIC_$argtype _gapic_tmp$k;\n";
            }
            print comp::PC "\tGAPIC_$argtype ";
            if($argmode eq "out") {
                print comp::PC "*";
            }
            print comp::PC "$argname";
            if($argmode eq "out") {
                print comp::PC "=&_gapic_tmp$k;\n";
            } elsif ($argtype eq "float") {
                print comp::PC "=ILP_get_double(machine,$k+1);\n";
            } elsif($argtype eq "atom") {
                print comp::PC "; ".$argname.".GAPIC_ILP_put_atom($k+1);\n";
            } else {
                print comp::PC "=ILP_get_integer(machine,$k+1);\n";
            }
	    } else {
	        printf "strange type $argtype\n";
        }
    }
    print comp::PC "gapic_global_machine=machine;\n{\n";
}

sub gapic_commit {
    print comp::PC "ilp_gapic_end_label: {};";
    for ($k=0; $k < $main::gapic_argcnt; $k++) {
        $argmode = $main::gapic_argmode[$k];
        $argtype = $main::gapic_argtype[$k];
        $argname = $main::gapic_argname[$k];
    	if($argmode eq "out") {
            if($argtype eq "float") {
                print comp::PC "\tif(!ILP_unify_double(machine,$k+1,_gapic_tmp$k))return 0;\n";
            } elsif($argtype eq "int") {
                print comp::PC "\tif(!ILP_unify_integer(machine,$k+1,_gapic_tmp$k))return 0;\n";
            } elsif($argtype eq "atom") {
                print comp::PC "\tif(!_gapic_tmp$k.ilp_unify_atom(machine,$k+1))return 0;\n";
            } elsif($argtype eq "term") {
                print comp::PC "\tif(!ILP_unify_term(machine,$k+1,$argname))return 0;\n";
            }
	    }
    }
    print comp::PC "\n}\nreturn GAPIC_SUCCESS;}\n";
}

return 1;
