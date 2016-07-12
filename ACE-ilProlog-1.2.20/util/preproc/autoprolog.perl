
sub convert_to_underscores {
	my ($input) = @_;
	my ($under, $i, $ch0, $ch1);
	$under = "";
	$len = length($input); $j = 0;
	for ($i = 0; $i <= $len; $i++) {
		$ch0 = substr($input, $i, 1);
		if ($ch0 ge "A" && $ch0 le "Z") {
			if ($i != 0) { $under = "$under\_"; }
			$ch1 = lc($ch0);
			$under = "$under$ch1";
		} else {
			$under = "$under$ch0";
		}
	}
	return $under;
}

sub auto_prolog_check_id {
	my ($id, $idname, $classname, $const, $method, $arg, $marg) = @_;
	if ($const eq "") { $call = "HeapGetPtrFromTerm"; } else { $call = "HeapGetDataFromTerm"; }
	if (($prolog_class_base eq "") || ($id ne "this_id")) {
		print AUTOPROLOG "\t$const$classname* $idname = $call<${classname}Collectable>($id);\n";
	} else {
		print AUTOPROLOG "\t$const$classname* $idname = ($classname*)$call<${prolog_class_base}Collectable>($id);\n";
	}
	print AUTOPROLOG "\tif ($idname == NULL) {\n";
	print AUTOPROLOG "\t\tcerr << \"$method/$arg - arg $marg: invalid c object ID \" << endl;\n";
	print AUTOPROLOG "\t\tGAPIC_fail;\n\t}\n";
}

sub auto_prolog_check_type {
	my ($aname, $atype, $method, $arg, $marg) = @_;
	if (($atype eq "int")
	    || ($atype eq "float")
	    || ($atype eq "atom")
	    || ($atype eq "term")) {
	    return;
	}
	print AUTOPROLOG "\tif (!GAPIC_is_$atype($aname)) {\n";
	print AUTOPROLOG "\t\tcerr << \"$method/$arg - arg $marg: invalid type - expacted $atype\" << endl;\n";
	print AUTOPROLOG "\t\tGAPIC_fail;\n\t}\n";
}

sub auto_prolog_pre_type {
	print AUTOPROLOG "\t";
	if ($returnobj == 1) {
		print AUTOPROLOG "$returntype* ret_ptr = ";
		return;
	}
	if (!($ap_res eq "")) {
		if ($ap_res eq "bool") {
			print AUTOPROLOG "if (!";
		} elsif($ap_res eq "GAPIC_result") {
		        print AUTOPROLOG "if(";
		} else {
			print AUTOPROLOG "*ret_val = ";
		}
	}
}

sub auto_prolog_post_type {
	if ($ap_res eq "bool") {
		print AUTOPROLOG ")) GAPIC_fail;\n";
	} elsif ($ap_res eq "GAPIC_result") {
	        print AUTOPROLOG ")!=GAPIC_SUCCESS)\n";
		print AUTOPROLOG "\t\tGAPIC_fail;\n";
	} else {
		print AUTOPROLOG ");\n";
		if ($returnobj == 1) {
			print AUTOPROLOG "\tif (ret_ptr != NULL) {\n";
			# Old code for use with int's (DO NOT REMOVE - USE LATER FOR UNSAFE VERSION)
			# print AUTOPROLOG "\t\t*ret_val = the_gapic_heap.Register(&ret_col);\n";
			# print AUTOPROLOG "\t\tCreateCollectable<${returntype}Collectable>(ret_col, ret_ptr);\n";
			print AUTOPROLOG "\t\tGAPIC_int ret_id = the_gapic_heap.Register(&ret_col);\n";
			print AUTOPROLOG "\t\tCreateCollectable<${returntype}Collectable>(ret_col, ret_ptr);\n";
			print AUTOPROLOG "\t\tret_val = IntToPrologID(ret_id);\n";
			print AUTOPROLOG "\t} else {\n";
			print AUTOPROLOG "\t\tGAPIC_fail;\n";
			print AUTOPROLOG "\t}\n";
		}
	}
}

sub auto_prolog {
    if ($line =~ s/virtual\s+//) { $virtual = "virtual "; } else { $virtual = ""; }
    if ($line =~ s/G_SO\s+//) { $g_so = "G_SO "; } else { $g_so = ""; }
    $line =~ /\s*(\S+)\s+(\S+)\((.*)\)(.*)/;
    $returntype = $1;
    $fct_name = $2;
    $arg_list = $3;
    $terminator = $4;
    $prolog_fct_name = convert_to_underscores($fct_name);
    $prolog_tot_name = "$prolog_class_name\_$prolog_fct_name";
    @args = split(/,\s*/, $arg_list);
    $nbarg = $#args+2;
    $returnobj = 0;
    if ($returntype =~ /int/) {
	    $ap_res = "int";
	    $nbarg++;
    } elsif ($returntype =~ /float/) {
	    $returntype = "double";
	    $ap_res = "float";
	    $nbarg++;
    } elsif ($returntype =~ /atom/) {
	    $ap_res = "atom";
	    $nbarg++;
    } elsif ($returntype =~ /bool/) {
	    $ap_res = "bool";
    } elsif ($returntype =~ /GAPIC_result/) {
	    $ap_res = "GAPIC_result";
    } elsif ($returntype =~ /void/) {
	    $ap_res = "";
    } else {				# Return type is object
	    # Old code for use with int's (DO NOT REMOVE - USE LATER FOR UNSAFE VERSION)
	    # $ap_res = "int";
	    $ap_res = "term";
	    $returnobj = 1;
	    $returntype =~ s/\*//;
	    $nbarg++;
    }
    if ($terminator =~ /const/) { $constfct = "const "; } else { $constfct = ""; }
    print AUTOPROLOG "#PRE#GAPIC_PRED $prolog_tot_name/$nbarg\n";
    print AUTOPROLOG "#PRE#GAPIC_ARG(this_id:term:in)\n";
    for ($k = 0; $k <= $#args; $k++) {
	    ($aname[$k], $atype[$k], $adir[$k]) = split(/:/, $args[$k]);
    	$mytype = $atype[$k]; $aobj[$k] = 0;
	    if ($mytype eq "") {				# Argument is an object
    	    $mytype = $aname[$k];
	        $mytype =~ s/\*//;
    	    if ($mytype =~ s/const\s+//) {
		        $aconst[$k] = "const ";
	        } else {
		        $aconst[$k] = "";
	        }
	        if ($mytype =~ /(\S+)\s+(\S+)/) {
		        $atype[$k] = $1;
		        $aname[$k] = $2;
		        $aobj[$k] = 1;
		        $adir[$k] = "in";
		        $mytype = "term";
	        }
	    }
	    print AUTOPROLOG "#PRE#GAPIC_ARG($aname[$k]";
    	print AUTOPROLOG ":$mytype:$adir[$k])\n";
    }
    if (!(($ap_res eq "") || ($ap_res eq "bool") || ($ap_res eq "GAPIC_result"))) {
	    print AUTOPROLOG "#PRE#GAPIC_ARG(ret_val:$ap_res:out)\n";
    }
    print AUTOPROLOG "#PRE#GAPIC_BEGINPRED\n";
    if ($returnobj == 1) {	print AUTOPROLOG "\tCollectable *ret_col;\n"; }
    if ($gapic_checking_level > 0) {
	    auto_prolog_check_id("this_id", "this_ptr", $classname, $constfct, $prolog_tot_name, $nbarg, 1);
	    for ($k = 0; $k <= $#args; $k++) {
	        if ($aobj[$k] == 0) {
		        if ($adir[$k] eq "in") {
    		        auto_prolog_check_type($aname[$k], $atype[$k], $prolog_tot_name, $nbarg, $k+2);
	    	    }
	        } else {
		        auto_prolog_check_id($aname[$k], "${aname[$k]}_ptr", $atype[$k], $aconst[$k], $prolog_tot_name, $nbarg, $k+2);
	        }
	    }
	    &auto_prolog_pre_type;
	    print AUTOPROLOG "this_ptr->$fct_name(";
    } else {
	    print AUTOPROLOG "cheap_id this_id=PrologIDToInt(this_id_term);\n";
	    for ($k = 0; $k <= $#args; $k++) {
    	    if (($aobj[$k] == 1)&&($adir[$k] eq "in")) {
		        print AUTOPROLOG "cheap_id $aname[$k]=";
		        print AUTOPROLOG "PrologIDToInt($aname[$k]"."_term);\n";
	        }
	    }
	    &auto_prolog_pre_type;
	    print AUTOPROLOG "UnsafeHeapGetPtr<${classname}Collectable>(this_id)->$fct_name(";
    }
    for ($k = 0; $k <= $#args; $k++) {
	    if ($k != 0) { print AUTOPROLOG ","; }
	    if ($aobj[$k] == 1) { print AUTOPROLOG "$aname[$k]_ptr"; }
	    else { print AUTOPROLOG $aname[$k]; }
    }
    &auto_prolog_post_type;
    print AUTOPROLOG "\tGAPIC_succeed;\n";
    print AUTOPROLOG "#PRE#GAPIC_ENDPRED\n\n";
    if ($returnobj == 1) { $returnptr = "*"; } else { $returnptr = ""; }
    $line = "\tprolog $g_so$virtual$returntype$returnptr $fct_name(";
    for ($k = 0; $k <= $#args; $k++) {
	    $mytype = $atype[$k];
	    if ($mytype eq "term") { $mytype = "GAPIC_term_ref"; }
	    elsif (($mytype eq "int")
	            || ($mytype eq "float")
    	        || ($mytype eq "atom")) {
	                $mytype = "GAPIC_$mytype";
        }
	    if ((($adir[$k] eq "out")
	            && (($mytype eq "GAPIC_int")
		        ||($mytype eq "GAPIC_float")
		        ||($mytype eq "GAPIC_atom")))
	            || ($aobj[$k] == 1)) {
	            $mytype = "$aconst[$k]$mytype*";
	    }
        if (($adir[$k] eq "in")&&($mytype eq "GAPIC_atom")) {
            $mytype = "const GAPIC_atom&"
        }
   	    if ($k != 0) {
   	        $line = "$line, $mytype $aname[$k]";
        } else {
            $line = "$line$mytype $aname[$k]";
   	    }
    }
    $line = "$line)$terminator\n";
}

sub auto_prolog_output {
	my($pos, $name) = @_;
	my($line);
	seek(TMPFILE, $pos, SEEK_SET);
	print MAINCFILE "// \$AUTO\$ $name\n";
	while ($line = <TMPFILE>) {
		if ($line =~ /\$AUTO\$/) { return; }
		print MAINCFILE $line;
	}
}

sub auto_prolog_main_file {
	my ($mainfile) = @_;
	my (@LINES, $i, $j, $id, $mine, @tmp_pos, @tmp_name, $tmp_idx);
	open(MAINCFILE, "$mainfile.C.gen") || die "Can't read main file: $mainfile.C.gen";
	$pos = 0;
	@LINES = <MAINCFILE>;
	close(MAINCFILE);
	open(MAINCFILE, ">$mainfile.C.gen") || die "Cen't create main file: $mainfile.C.gen";
	$crpos = 0;
	for ($i = 0; $i <= $#LINES; $i++) {
		if ($LINES[$i] =~ /\$AUTO\$/) {
			$crpos = $i+1;
			$i = $#LINES+1;
		} else {
			print MAINCFILE $LINES[$i];
		}
	}
	print MAINCFILE "// \$AUTO\$ This part is auto-generated -- do not edit\n";
	print MAINCFILE "extern \"C\" {\n\n";
	if (open(TMPFILE, "$mainfile.tmp")) {
		while ($mine = <TMPFILE>) {
			chop $mine;
			if ($mine =~ /\$AUTO\$\s*(\S+)/) {
				$tmp_name[$tmp_idx] = $1;
				$tmp_pos[$tmp_idx++] = tell(TMPFILE);
			}
		}
		$out_line = 0;
		for ($i = $crpos; $i < $#LINES; $i++) {
			if ($LINES[$i] =~ /\$AUTO\$\s*(\S+)/) {
				$id = $1;
				$out_line = 1;
				for ($j = 0; $j < $tmp_idx; $j++) {
					if ($id eq $tmp_name[$j]) {
						if ($tmp_pos[$j] != -1) {
							auto_prolog_output($tmp_pos[$j], $tmp_name[$j]);
							$tmp_pos[$j] = -1;
						}
						$out_line = 0;
						$j = $tmp_idx;
					}
				}
			}
			if ($LINES[$i] =~ /\$ENDAUTO\$/) {
				$i = $#LINES+1;
			} elsif ($out_line == 1) {
				print MAINCFILE $LINES[$i];
			}
		}
		for ($j = 0; $j < $tmp_idx; $j++) {
			if ($tmp_pos[$j] != -1) {
				auto_prolog_output($tmp_pos[$j], $tmp_name[$j]);
			}
		}
		close(TMPFILE);
	} else {
		die "Can't open $mainfile.tmp\n";
	}
	print MAINCFILE "// \$ENDAUTO\$\n}\n";
	close(MAINCFILE);
	unlink "$mainfile.tmp";
}

sub clear_prolog_main_files {
    $#mainfiles = -1;
}

sub close_auto_prolog {
   	if ($auto_prolog == 1) {
	    	close(AUTOPROLOG);
	}
}

sub autoprolog_main_file {
    my ($mf) = @_;
    $mainfile = $mf;
    $auto_prolog = 1;
    $fname = "$generic_dir/$subdir$mainfile.tmp";
    $midx = $#mainfiles+1;
    $mname = "$generic_dir/$subdir$mainfile";
    for ($j = 0; $j <= $#mainfiles; $j++) {
        if ($mname eq $mainfiles[$j]) { $midx = -1; }
    }
    if ($midx != -1) {
        $mainfiles[$midx] = $mname;
    }
    if (open(AUTOPROLOG, ">>$fname")) {
        print AUTOPROLOG "// \$AUTO\$ $name\n";
    } else {
        die "Can't open C main file: $fname\n";
    }
}

return 1;
