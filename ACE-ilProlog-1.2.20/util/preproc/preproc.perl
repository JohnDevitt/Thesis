#!/usr/bin/perl

require "$UTIL_DIR/preproc/autoprolog.perl";
require "$UTIL_DIR/preproc/gapic.perl";
require "$UTIL_DIR/preproc/compiler/compiler.perl";


@comp_suffix = (".pro", ".pl", ".yap", ".ilp");

$FTYPE_PROLOG = 0;
$FTYPE_LPD    = 1;
$FTYPE_HEADER = 2;
$FTYPE_C      = 3;

$PREPROC_ALL  = 0;

$comp_index{"master"}  = 0;
$comp_index{"sicstus"} = 1;
$comp_index{"yap"}     = 2;
$comp_index{"ilp"}     = 3;

$is_os{"LINUX"}   = 1;
$is_os{"SOLARIS"} = 1;
$is_os{"WINDOWS"} = 1;
$is_os{"UNIX"}    = 1;
$is_os{"MACOS"}   = 1;

$os_is_a{"LINUX"}   = "UNIX";
$os_is_a{"SOLARIS"} = "UNIX";
$os_is_a{"MACOS"}   = "UNIX";

sub select_compiler {
    my ($comp) = @_;
    require "$UTIL_DIR/preproc/compiler/${comp}.perl";
    $cc = eval("new ${comp}();");
    $uc_compiler = uc($compiler);
    $uc_os       = uc($aceos);
    $cc->create_cpreproc("$UTIL_DIR/preproc/c", "$ACE_DIR/V-$version/src/ace.pch");
}

sub set_second_pass_file {
    my ($sfile) = @_;
    $SFILE[$NB_SECOND_PASS++] = $sfile;
}

sub terminate_preproc {
    $cc->get_second_pass_files();
    if ($NB_SECOND_PASS > 0) {
        print "Second pass\n";
		$prevdir = "";
        for ($i = 0; $i < $NB_SECOND_PASS; $i++) {
	        if ($SFILE[$i] =~ /(.+\/)([^\/]+)$/) {
		        $subdir = $1;
		        $name = $2;
			} else {
				$subdir = "";
				$name = $SFILE[$i];
			}
	        $name =~ s/\.gen//;
	        if ($prevdir ne $subdir) {
	    	    $file_changed = 0;
	        }
	        process_one($name, 0);
	        $prevdir = $subdir;
        }
    }
    $cc->save_cpreproc("$ACE_DIR/V-$version/src/ace.pch");
    if ($cc->has_error_cpreproc() == 1) {
        $cc->remove_cpreproc();
    	exit(-1);
    }
    $cc->remove_cpreproc();
}

sub run_preproc {
    ($maindir, $version, $subdir) = @_;

    $systemdir = "$maindir/V-$version";
    if (!($subdir eq "")) { $subdir = $subdir . "/"; }
    opendir thisdir, "$systemdir/src/$subdir";
    @allfiles = grep /^[^\.].*\.gen$/, readdir thisdir;
    closedir thisdir;

    $generic_dir  = "$systemdir/src";
    $specific_dir = "$systemdir/src.$compiler/$aceos";
    $cindex       = $comp_index{$compiler};
    $suffix       = $comp_suffix[$cindex];
    clear_prolog_main_files();

    $file_changed = 0;

    # $nothing_changed could be used to check whether links have to be redone;
    # it is not really used at this moment though
    $nothing_changed = "true";

    # generate runtime type checking code?
    $gapic_checking_level = 1;

    # first preproc header files
    foreach (@allfiles) {
        ($name) = /(.*)\.gen$/;
        if ($name =~ /\.h$/) { process_one($name, 1); }
    }

    # update main C++ files
    for ($i = 0; $i <= $#mainfiles; $i++) {
    	auto_prolog_main_file($mainfiles[$i]);
    }
    clear_prolog_main_files();

    # after that preproc other files
    foreach (@allfiles) {
        ($name) = /(.*)\.gen$/;
        if (!($name =~ /\.h$/)) { process_one($name, 1); }
    }
}

sub show_changed_message {
    if ($file_changed == 0) {
        $file_changed = 1;
    	print "Generating files: $generic_dir/$subdir\n";
    }
}

sub check_recent {
    @dummy = stat "$fullname";                    # get file stats
    $min = $dummy[9];                             # extract modification time
    if (check_suffix($name, $min) == 0) {
        # targets up to date
        if ($SHOW_SKIPPING == 1) {
	    show_changed_message();
	    print "  skipping $name.gen\n";
        }
    	return 0;
    } else {
	show_changed_message();
        # targets outdated : recompile
	print "  processing $name.gen\n";
	return 1;
    }
}

sub init_vars {
	$ftype = $FTYPE_PROLOG;
	$nothing_changed = "false";
	$linenr = 0;
	$disable = 0;
	%debug = ();
	init_gapic();
}

sub on_special_ftype {
    if ($name =~ /\.h$/) {
	$ftype = $FTYPE_HEADER;
	$classname = "";
	$prolog_class_count = 1;
    } elsif ($name =~ /\.lpd$/) {
        $ftype = $FTYPE_LPD;
    } else {
        $ftype = $FTYPE_C;
    }
}

sub open_name {
	if (($name =~ /\.c$/) || ($name =~ /\.C$/) || ($name =~ /\.h$/)) {
		on_special_ftype();
	    $cc->open_output_c($name, "$specific_dir/$name");
	} else {
	    if ($name =~ /\.lpd$/) {
	        $cc->open_output_prolog($name, "$specific_dir/$name");
	    } else {
	        $cc->open_output_prolog("$name.gen", "$specific_dir/$name$suffix");
	        $cc->prolog_header();
	    }
	}
}

sub process_one {
    my ($shortname, $docheck) = @_;
    $name = $subdir . $name;
    $fullname = "$generic_dir/$name.gen";
    if ($docheck == 1) {
    	if (check_recent() == 0) {
	        return;
		}
    } else {
    	show_changed_message();
		print "  processing $name.gen\n";
    }
    if (!open(GC, "$fullname")) {
		print "Can't open: $fullname\n";
		$cc->cant_open("$name.gen");
		return;
	}
    init_vars();	
    open_name();	
    while ($line = <GC>) {
        chop($line);
        $linenr++;
        process_line();
        # specific processing moved to C++ code
        if ($disable == 0) {
            $cc->write_line();
        } else {
            $cc->write_nl();
        }
    }
	close(GC);		
	close_gapic();
    close_auto_prolog();
    $cc->second_pass($shortname, $subdir);
    $cc->close_output();
}

sub do_hash_signs {
    if ($line =~ /^\s*[%#]IFDEBUG[%#](.*?)[%#](.*)/) {
        $debugtest = $1;
        $debugline = $2;
        if($debug{$debugtest}==1) {
		    $line = "$2";
        } else {
            $line = "";
        }
    } elsif($line =~ /^[%#]DEBUGON[%#](.*?)[%#]/) {
        $debugtest = $1;
        $debug{$debugtest}=1;
        printf "Debugging $debugtest \n";
        $line = "";
    } elsif($line =~ /^\#PRE\#GAPIC/) {
        do_gapic();
    } elsif ($line =~ /^[%#]PRE[%#]([\w\s]+)/) {
        $pre_name = uc($1);
        if ($pre_name =~ /ALL/) {
            $disable = 0;
        } else {
            $isin = 0;
            @to_enable = split(/\s+/, $pre_name);
	    if ($is_os{$to_enable[0]}) {
	        for ($q = 0; $q <= $#to_enable; $q++) {
                    if (($to_enable[$q] eq $uc_os) || ($to_enable[$q] eq $os_is_a{$uc_os})) { $isin = 1; }
		}
                if ($isin != 1) { $disable |= 1; }
                else { $disable &= (0xfffff ^ 1); }
            } else {
                for ($q = 0; $q <= $#to_enable; $q++) {
                    if ($uc_compiler eq $to_enable[$q]) { $isin = 1; }
                }
                if ($isin != 1) { $disable |= 2; }
                else { $disable &= (0xfffff ^ 2); }
            }
	}
        $line = "";
    }
}

sub do_header_file {
	    # Header file - support of prolog functions
		if ($line =~ /^\s*prolog(.*)/) {
			$line = $1;
			$prolog_class_count++;
			auto_prolog();
		}
		if ($line =~ /^class\s+(\S+)/) {
			$classname = $1;
			if ($classname eq "CLASS_SO") {
				$line =~ /CLASS\_SO\s+(\S+)/;
				$classname = $1;			
			}
			if ($prolog_class_count > 0) {
				$prolog_class_name = convert_to_underscores($classname);
				$prolog_class_base = "";
			}
		}
}

sub process_line {
    if ($line =~ /[%#]/) {
        do_hash_signs();
    }
    if ($ftype == $FTYPE_HEADER) {
        do_header_file();
    }
    if ($gapic == 2) {
	gapic_findparam;
    }
}

sub check_suffix {
	my ($name, $min) = @_;
	if (($name =~ /\.c$/)||($name =~ /\.C$/)||
	    ($name =~ /\.lpd$/)||($name =~ /\.h$/)) {
		@dummy = stat "$specific_dir/$name";
	} else {
		@dummy = stat "$specific_dir/$name$suffix";
	}
	if (($min > $dummy[9]) || ($PREPROC_ALL == 1)) {
		return 1;
	} else {
		return 0;
	}
}

return 1;
