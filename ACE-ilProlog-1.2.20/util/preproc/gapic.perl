
sub init_gapic {
    $gapic = 0;
    $auto_prolog = 0;
    $gapic_gluecode = 0;
    $prolog_class_base = "";
}

sub close_gapic {
    if (($gapic > 0) && ($gapic_gluecode == 1)) {
        if ($gapic > 1) {
            $cc->close_gapic_glue();
            $cc->close_output();
            die  "Error: end_of_file before Gapic predicate $gapic_predname ".
                 "finished\n";
        }
        $cc->gapic_footer();
        $cc->close_gapic_glue();
        if ($module_id =~ /stdgapic/) {
             return;
        }
        print comp::PC "\nextern \"C\" {\n";
        print comp::PC "SOPROVIDE GAPIC_result gapic_module_init_$module_id() {\n";
        print comp::PC "  void *stdgapic;\n";
        print comp::PC "  void *localso;\n";
        print comp::PC "  void (*localinit)();\n";
        if ($debug{"CIFACE"}) {
            print comp::PC "  printf(\"%s : Init ... \",GAPIC_MODULE_STR);\n";
        }
        print comp::PC "  #ifdef init_module\n";
        print comp::PC "    init_module();\n";
        print comp::PC "  #endif\n";
        if ($debug{"CIFACE"}) {		
            print comp::PC "  printf(\" done.\\n\");\n";
        }
        print comp::PC "  GAPICRegisterAllTypes();\n";
        print comp::PC "  return GAPIC_SUCCESS;\n";
        print comp::PC "}\n";
        print comp::PC "}\n";
    }
}

sub gapic_begin {
    if ($gapic_argcnt != $gapic_arity) {
		die "Line $linenr: GAPIC arity does not agree ".
		    "with actual number of registered arguments ".
		    "($gapic_argcnt != $gapic_arity)\n";
    }
    $cc->gapic_begin();
}

sub gapic_def_object {
    my ($moddir, $modname) = @_;
    $c_module_dir = $moddir;
    $c_module_name = $modname;
    if($gapic == 1) {
        print "Line $linenr: Only one GAPIC_MODULE or GAPIC_OBJECT in file allowed\n";
    } elsif ($ftype != $FTYPE_C) {
        print "Line $linenr: GAPIC directives are only allowed in .c files\n";
    } else {
        if($line =~ /GAPIC_MODULE/) {
        	$gapic_gluecode=1;
        }
        $gapic=1;
        $choppedname = $c_module_dir . "/" . $c_module_name;
        print comp::PC "#define COMPILER_${uc_compiler}\n";
        if ($gapic_gluecode) {
            $module_id = $choppedname;
            $module_id =~ s/[^-\.\d\w]/_/g;
            $gapic_list = "gapic_module_init";
            print comp::PC "#define GAPIC_MODULE $choppedname\n";
            print comp::PC "#define GAPIC_MODULE_ID $module_id\n";
            print comp::PC "#define GAPIC_MODULE_STR \"$choppedname\"\n";
            $cc->open_gapic_glue();
            $cc->gapic_print_header();
        } else {
            print comp::PC "#define GAPIC_MODULE";
        }
    }
}

sub gapic_findparam {
	while($line =~ /(.*)\#PRE\#GAPIC_ARG\s*\(\s*(\S+)\s*:\s*(\S+)\s*:\s*(\S+)\s*\)(.*)/) {
		$line = $1.$5;
		$gapic_argname[$gapic_argcnt]=$2;
		$gapic_argtype[$gapic_argcnt]=$3;
		$gapic_argmode[$gapic_argcnt]=$4;
		if(($gapic_argtype[$gapic_argcnt]!="float")&&
		   ($gapic_argtype[$gapic_argcnt]!="int")&&
		   ($gapic_argtype[$gapic_argcnt]!="term")) {
			die "Line $linenr, pred $gapic_predname,".
		        "arg $gapic_argcnt: unsupported type ".
		        "$gapic_argtype[$gapic_argcnt]\n";
		}
		if(($gapic_argmode[$gapic_argcnt]!="in")&&
		   ($gapic_argmode[$gapic_argcnt]!="out")) {
			die "Line $linenr, pred $gapic_predname, ".
		        "arg $gapic_argcnt: unsupported mode ".
		        "$gapic_argmode[$gapic_argcnt]\n";
		}
		# print "GAPIC_ARG $gapic_argname[$gapic_argcnt]::$gapic_argtype[$gapic_argcnt]\n";
		$gapic_argcnt++;
	}
}

sub gapic_pred {
    if (!($gapic_gluecode)) {
        die "Line $linenr: GAPIC_PRED directives ".
            "in a gapic object with no glue code generation\n";
    } elsif ($gapic == 0) {
        die "Line $linenr: GAPIC_PRED directives ".
        	"should be preceded by GAPIC_MODULE\n";
    } elsif ($gapic == 2) {
        die "Line $linenr: GAPIC_PRED found while ".
            "previous GAPIC_PRED still active\n";
    } elsif($line =~ /^\#PRE\#GAPIC_PRED\s+(\S+)\s*\/\s*(\d+)/) {
        $gapic_functor = $1;
        $gapic_predname = $1;
        $gapic_arity = $2;
        $gapic = 2;
        $gapic_argcnt = 0;
        $gapic_list = $gapic_list.",".$gapic_predname;
        # print "GAPIC_PRED $gapic_predname\n";
    } else {
        die "Line $linenr: GAPIC_PRED should be followed ".
            "by functor/arity\n";
    }
}

sub gapic_begin_pred {
    if(!($gapic_gluecode)) {
        die "Line $linenr: GAPIC_PRED directives ".
        	"in a gapic object with no glue code generation\n";
    } elsif ($gapic < 2) {
        die "Line $linenr: GAPIC_BEGINPRED while no GAPIC_PRED active\n";
    } elsif ($gapic > 2) {
        die "Line $linenr: second GAPIC_BEGINPRED found\n";
    } else {
        gapic_begin();
        $gapic = 3;
    }
}

sub gapic_end_pred {
    if (!($gapic_gluecode)) {
        die "Line $linenr: GAPIC_PRED directives ".
        	"in a gapic object with no glue code generation\n";
    } elsif ($gapic < 3) {
        die "Line $linenr: GAPIC_ENDPRED ".
        	"while no GAPIC_BEGINPRED active\n";
    } else {
        $cc->gapic_commit();
        $gapic = 1;
    }
}

sub do_gapic {
    if (($gapic == 2) && ($line =~ /GAPIC_ARG/)) {
        gapic_findparam();
        return;
    }
    if (($line =~ /^\#PRE\#GAPIC_OBJECT\#(.*)\#(.*)\#/)||
           ($line =~ /^\#PRE\#GAPIC_MODULE\#(.*)\#(.*)\#/)) {
        gapic_def_object($1, $2);
        $line = "";
    } elsif ($line =~ /^\#PRE\#GAPIC_PRED/) {
        gapic_pred();
        $line = "";
    } elsif ($line =~ /^\#PRE\#GAPIC_BEGINPRED/) {
        gapic_begin_pred();
        $line = "";
    } elsif($line =~ /^\#PRE\#GAPIC_ENDPRED/) {
        gapic_end_pred();
        $line = "";
    } elsif($line =~ /^\#PRE\#GAPIC_MAIN_FILE\s+(\S+)/) {
        autoprolog_main_file($1);
        $line = "";
    } elsif($line =~ /^\#PRE\#GAPIC_CHECKING_LEVEL\s+(\d+)/) {
        $gapic_checking_level = $1;
        $line = "";
    } elsif($line =~ /^\#PRE\#GAPIC_CLASS\s+(\S+)/) {
        $prolog_class_name = $1;
        $prolog_class_count = 0;
    } elsif($line =~ /^\#PRE\#GAPIC_BASE\s+(\S+)/) {
        $prolog_class_base = $1;
        $line = "";
    } elsif($line =~ /^\#PRE\#GAPIC_BASE\s*$/) {
        $prolog_class_base = "";
        $line = "";
    } else {
        printf ">> $line\n";
        die "Line $linenr: unknown \#PRE\#GAPIC... directive\n";
    }
}

return 1;
