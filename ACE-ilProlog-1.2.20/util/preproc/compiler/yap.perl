
package yap;

# FIXME - gapic is broken :-(

@ISA = qw(comp);

@FROM = (
    '%PRE%user:([^%]*)%',
    '#PRE#user:([^%]*)#',
    '%PRE%([^%]*)\$([^%]*)%',
    '#PRE#([^%]*)\$([^%]*)#',
    '%PRE%PROLOG%', 
    'my_use_module',
    'my_compile'
);

@TO = (
 'user:$1',
 'user:$1',
 '$2:$1',
 '$2:$1',
 'Yap', 
 'use_module',
 'my_compile'
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
    return ".C";
}

sub gapic_print_header {
    print comp::GGLUE "\nextern \"C\" {\n";
    print comp::GGLUE "\nvoid init_${main::c_module_name}_preds() {\n";
}

sub gapic_footer {
	print comp::PC "\n#include \"$main::c_module_name.glue.C\"\n";		
	print comp::GGLUE "\tgapic_module_init();\n";		
	print comp::GGLUE "}\n\n}\n";			
}

sub gapic_begin {
    print YapGlue "\tUserCPredicate(\"$gapic_predname\", $gapic_predname, $gapic_argcnt);\n";
    print Yap "int $gapic_predname() {\n";
    for($k=0;$k<$gapic_argcnt;$k++) {
	$aid = $k+1;    
	if ($gapic_argtype[$k] eq "term") {
		if ($gapic_argmode[$k] eq "out") {
			print Yap "\tGAPIC_term_ref $gapic_argname[$k] = GAPIC_new_term_ref();\n";
			print Yap "\tTerm _gapic_arg_term$k = ARG$aid;\n";
		} else {
			print Yap "\tGAPIC_term_ref $gapic_argname[$k] = GAPIC_new_term_ref();\n";
			print Yap "\t*MY_YAP_get_term($gapic_argname[$k]) = Deref(ARG$aid);\n";
		}
	} elsif (($gapic_argtype[$k] eq "int") || ($gapic_argtype[$k] eq "float") || ($gapic_argtype[$k] eq "atom")) { 
		if ($gapic_argmode[$k] eq "out") {
			print Yap "\tGAPIC_$gapic_argtype[$k] _gapic_tmp$k;\n";
			print Yap "\tTerm _gapic_arg_term$k = ARG$aid;\n";
			print Yap "\tTerm _gapic_var_term$k;\n";
		} else {
			print Yap "\tTerm _gapic_arg_term$k = ARG$aid;\n";		
		}
		print Yap "\tGAPIC_$gapic_argtype[$k] ";
		if ($gapic_argmode[$k] eq "out") {
			print Yap "*";
		}
		$myterm = "Deref(_gapic_arg_term$k)";
		print Yap "$gapic_argname[$k]";
		if ($gapic_argmode[$k] eq "out") {
			print Yap "=&_gapic_tmp$k;\n";
		} elsif ($gapic_argtype[$k] eq "float") {
			print Yap "=FloatOfTerm($myterm);\n";
		} elsif ($gapic_argtype[$k] eq "atom") {
			print Yap "=AtomOfTerm($myterm);\n";
		} else {
			print Yap "=IntOfTerm($myterm);\n";
		}
	}
    }
}

sub gapic_commit {
    print Ilp "ilp_gapic_end_label: {};";
    for($k=0;$k<$gapic_argcnt;$k++) {
	if($gapic_argmode[$k] eq "out") {
            if($gapic_argtype[$k] eq "float") {
		print Ilp "\tILP_unify_double(machine,$k+1,_gapic_tmp$k);\n";}
            elsif($gapic_argtype[$k] eq "int") {
		print Ilp "\tILP_unify_integer(machine,$k+1,_gapic_tmp$k);\n";}
            elsif($gapic_argtype[$k] eq "atom") {
		print Ilp "\t_gapic_tmp$k.ilp_unify_atom(machine,$k+1);\n";}
	    elsif($gapic_argtype[$k] eq "term") {
		print Ilp "\tILP_unify_term(machine,$k+1,$gapic_argname[$k]);\n";}
	}
    }
    print Ilp "\n}\nreturn GAPIC_SUCCESS;}\n";
    
    print Yap "yap_gapic_yes_label: {};\n";
    for($k=0;$k<$gapic_argcnt;$k++) {
	if($gapic_argmode[$k] eq "out") {
		$argterm = "_gapic_arg_term$k";	
		if ($gapic_argtype[$k] eq "term") {
			if ($YAP_NEW_UNIFY == 1) {
				print Yap "\tunify(*MY_YAP_get_term($gapic_argname[$k]), $argterm);\n";
			} else {
				print Yap "\tunify(MY_YAP_get_term($gapic_argname[$k]), &$argterm);\n";					
			}
		} else {		
			$varterm = "_gapic_var_term$k";			
			$tmpterm = "_gapic_tmp$k";				
			if ($gapic_argtype[$k] eq "float") {
				print Yap "\t$varterm = MkFloatTerm($tmpterm);\n";
			} elsif ($gapic_argtype[$k] eq "int") {
				print Yap "\t$varterm = MkIntTerm($tmpterm);\n";
			} elsif ($gapic_argtype[$k] eq "atom") {
				print Yap "\t$varterm = MkAtomTerm($tmpterm.Get());\n";
			} 
			if ($YAP_NEW_UNIFY == 1) {
				print Yap "\tunify($varterm, $argterm);\n";
			} else {
				print Yap "\tunify(&$varterm, &$argterm);\n";
			}				
		}
	}    
    }
    print Yap "\tMY_YAP_free_terms();\n";
    print Yap "\treturn GAPIC_SUCCESS;\n";
    print Yap "yap_gapic_no_label: {};\n";    
    print Yap "\tMY_YAP_free_terms();\n";
    print Yap "\treturn GAPIC_FAIL;\n}\n";
     
    print Sicstus "}\n";    
}

return 1;
