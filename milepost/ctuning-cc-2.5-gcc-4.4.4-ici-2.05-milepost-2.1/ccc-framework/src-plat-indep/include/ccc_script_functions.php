<?php

# Copyright (C) 2004-2009 by Grigori Fursin
#
# http://fursin.net/research
#
# UNIDAPT Group
# http://unidapt.org

/************************************************************************************************/
function get_num_program_distinct($link)
{
  $num=0;

  $query="SELECT DISTINCT PROGRAM_ID FROM STATS_EXEC_GLOBAL";
  $result=mysql_query($query, $link);
  if ($result)
  {
    $num=mysql_numrows($result);
    mysql_free_result($result);
  }

  return $num;
}

/************************************************************************************************/
function get_num_program($link)
{
  $num=0;

  $query="SELECT PROGRAM_ID FROM PROGRAMS";
  $result=mysql_query($query, $link);
  if ($result)
  {
    $num=mysql_numrows($result);
    mysql_free_result($result);
  }

  return $num;
}

/************************************************************************************************/
function get_num_platform_distinct($link)
{
  $num=0;

  $query="SELECT DISTINCT PLATFORM_ID FROM STATS_EXEC_GLOBAL";
  $result=mysql_query($query, $link);
  if ($result)
  {
    $num=mysql_numrows($result);
    mysql_free_result($result);
  }

  return $num;
}

/************************************************************************************************/
function get_num_platform($link)
{
  $num=0;

  $query="SELECT PLATFORM_ID FROM PLATFORMS";
  $result=mysql_query($query, $link);
  if ($result)
  {
    $num=mysql_numrows($result);
    mysql_free_result($result);
  }

  return $num;
}

/************************************************************************************************/
function get_num_platform_features_distinct($link)
{
  $num=0;

  $query="SELECT DISTINCT PLATFORM_FEATURE_ID FROM STATS_COMP_GLOBAL_FLAGS";
  $result=mysql_query($query, $link);
  if ($result)
  {
    $num=mysql_numrows($result);
    mysql_free_result($result);
  }

  return $num;
}

/************************************************************************************************/
function get_num_platform_configurations($link)
{
  $num=0;

  $query="SELECT PLATFORM_FEATURE_ID FROM PLATFORM_FEATURES";
  $result=mysql_query($query, $link);
  if ($result)
  {
    $num=mysql_numrows($result);
    mysql_free_result($result);
  }

  return $num;
}

/************************************************************************************************/
function get_num_environment_distinct($link)
{
  $num=0;

  $query="SELECT DISTINCT ENVIRONMENT_ID FROM STATS_EXEC_GLOBAL";
  $result=mysql_query($query, $link);
  if ($result)
  {
    $num=mysql_numrows($result);
    mysql_free_result($result);
  }

  return $num;
}

/************************************************************************************************/
function get_num_environment($link)
{
  $num=0;

  $query="SELECT ENVIRONMENT_ID FROM ENVIRONMENTS";
  $result=mysql_query($query, $link);
  if ($result)
  {
    $num=mysql_numrows($result);
    mysql_free_result($result);
  }

  return $num;
}

/************************************************************************************************/
function get_num_compiler_distinct($link)
{
  $num=0;

  $query="SELECT DISTINCT COMPILER_ID FROM STATS_COMP_GLOBAL_FLAGS";
  $result=mysql_query($query, $link);
  if ($result)
  {
    $num=mysql_numrows($result);
    mysql_free_result($result);
  }

  return $num;
}

/************************************************************************************************/
function get_num_compiler($link)
{
  $num=0;

  $query="SELECT COMPILER_ID FROM COMPILERS";
  $result=mysql_query($query, $link);
  if ($result)
  {
    $num=mysql_numrows($result);
    mysql_free_result($result);
  }

  return $num;
}

/************************************************************************************************/
function get_num_executions($link)
{
  $num=0;

  $query="SELECT COUNT(*) FROM STATS_EXEC_GLOBAL";
  $result=mysql_query($query, $link);
  if ($result)
  {
    $row=mysql_fetch_array($result);
    $num=$row['COUNT(*)'];
    mysql_free_result($result);
  }

  return $num;
}

/************************************************************************************************/
function get_num_compilations($link)
{
  $num=0;

  $query="SELECT COUNT(*) FROM STATS_COMP_GLOBAL_FLAGS";
  $result=mysql_query($query, $link);
  if ($result)
  {
    $row=mysql_fetch_array($result);
    $num=$row['COUNT(*)'];
    mysql_free_result($result);
  }

  return $num;
}

/************************************************************************************************/
function get_num_compilations_with_ici($link)
{
  $num=0;

  $query="SELECT COUNT(*) FROM STATS_COMP_GLOBAL_FLAGS WHERE ICI_PASSES_USE=1";
  $result=mysql_query($query, $link);
  if ($result)
  {
    $row=mysql_fetch_array($result);
    $num=$row['COUNT(*)'];
    mysql_free_result($result);
  }

  return $num;
}

/************************************************************************************************/
function get_num_compilations_with_feat($link)
{
  $num=0;

  $query="SELECT COUNT(*) FROM STATS_COMP_GLOBAL_FLAGS WHERE ICI_FEATURES_STATIC_EXTRACT=1";
  $result=mysql_query($query, $link);
  if ($result)
  {
    $row=mysql_fetch_array($result);
    $num=$row['COUNT(*)'];
    mysql_free_result($result);
  }

  return $num;
}

/************************************************************************************************/
function get_num_compilations_distinct($link)
{
  $num=0;

  $query="SELECT COUNT(DISTINCT STATS_COMP_GLOBAL_FLAGS.OBJ_MD5CRC) FROM STATS_COMP_GLOBAL_FLAGS";
  $result=mysql_query($query, $link);
  if ($result)
  {
    $row=mysql_fetch_array($result);
    $num=$row['COUNT(DISTINCT STATS_COMP_GLOBAL_FLAGS.OBJ_MD5CRC)'];
    mysql_free_result($result);
  }

  return $num;
}

/************************************************************************************************/
function get_num_datasets_distinct($link)
{
  $num=0;

  $query="SELECT COUNT(DISTINCT DATASET_ID) FROM STATS_EXEC_GLOBAL";
  $result=mysql_query($query, $link);
  if ($result)
  {
    $row=mysql_fetch_array($result);
    $num=$row['COUNT(DISTINCT DATASET_ID)'];
    mysql_free_result($result);
  }

  return $num;
}

/************************************************************************************************/
function get_num_opt_flags_global_distinct($link)
{
  $num=0;

  $query="SELECT COUNT(DISTINCT STATS_COMP_GLOBAL_FLAGS.OPT_ID) FROM STATS_COMP_GLOBAL_FLAGS";
  $result=mysql_query($query, $link);
  if ($result)
  {
    $row=mysql_fetch_array($result);
    $num=$row['COUNT(DISTINCT STATS_COMP_GLOBAL_FLAGS.OPT_ID)'];
    mysql_free_result($result);
  }

  return $num;
}

/************************************************************************************************/
function get_num_opt_flags_global($link)
{
  $num=0;

  $query="SELECT OPT_ID FROM OPT_FLAGS_GLOBAL";
  $result=mysql_query($query, $link);
  if ($result)
  {
    $num=mysql_numrows($result);
    mysql_free_result($result);
  }

  return $num;
}

/************************************************************************************************/
function get_num_opt_flags_platforms_global($link)
{
  $num=0;

  $query="SELECT OPT_PLATFORM_ID FROM OPT_FLAGS_GLOBAL_PLATFORM";
  $result=mysql_query($query, $link);
  if ($result)
  {
    $num=mysql_numrows($result);
    mysql_free_result($result);
  }

  return $num;
}

/************************************************************************************************/
function get_num_opt_passes($link)
{
  $num=0;

  $query="SELECT OPT_PASSES_ID FROM OPT_PASSES";
  $result=mysql_query($query, $link);
  if ($result)
  {
    $num=mysql_numrows($result);
    mysql_free_result($result);
  }

  return $num;
}

/************************************************************************************************/
function get_inum_comp($prog_id, $link)
{
  $num=0;

  $query="SELECT COUNT(*) FROM STATS_COMP_GLOBAL_FLAGS  WHERE " . 
                  "(PROGRAM_ID=" . $prog_id . ")";
  $result=mysql_query($query, $link);
  if ($result)
  {
    $row=mysql_fetch_array($result);
    $num=$row['COUNT(*)'];
    mysql_free_result($result);
  }

  return $num;
}

/************************************************************************************************/
function get_inum_comp_distinct($prog_id, $link)
{
  $num=0;

  $query="SELECT COUNT(DISTINCT OBJ_MD5CRC) FROM STATS_COMP_GLOBAL_FLAGS  WHERE " . 
                  "(PROGRAM_ID=" . $prog_id . ")";
  $result=mysql_query($query, $link);
  if ($result)
  {
    $row=mysql_fetch_array($result);
    $num=$row['COUNT(DISTINCT OBJ_MD5CRC)'];
    mysql_free_result($result);
  }

  return $num;
}

/************************************************************************************************/
function get_inum_run($prog_id, $link)
{
  $num=0;

  $query="SELECT COUNT(*) FROM STATS_EXEC_GLOBAL  WHERE " . 
                  "(PROGRAM_ID=" . $prog_id . ")";
  $result=mysql_query($query, $link);
  if ($result)
  {
    $row=mysql_fetch_array($result);
    $num=$row['COUNT(*)'];
    mysql_free_result($result);
  }

  return $num;
}

/************************************************************************************************/
function get_inum_run_bad($prog_id, $time_thr, $link)
{
  $num=0;

  $query="SELECT COUNT(*) FROM STATS_EXEC_GLOBAL  WHERE " . 
                  "(PROGRAM_ID=" . $prog_id . ")" .
                  "AND (STATS_EXEC_GLOBAL.RUN_TIME <=" . $time_thr . ")";
  $result=mysql_query($query, $link);
  if ($result)
  {
    $row=mysql_fetch_array($result);
    $num=$row['COUNT(*)'];
    mysql_free_result($result);
  }

  return $num;
}

/************************************************************************************************/
function get_inum_platforms($prog_id, $link)
{
  $num=0;

  $query="SELECT COUNT(DISTINCT PLATFORM_ID) FROM STATS_EXEC_GLOBAL  WHERE " . 
                  "(PROGRAM_ID=" . $prog_id . ")";
  $result=mysql_query($query, $link);
  if ($result)
  {
    $row=mysql_fetch_array($result);
    $num=$row['COUNT(DISTINCT PLATFORM_ID)'];
    mysql_free_result($result);
  }

  return $num;
}

/************************************************************************************************/
function get_inum_environments($prog_id, $link)
{
  $num=0;

  $query="SELECT COUNT(DISTINCT ENVIRONMENT_ID) FROM STATS_EXEC_GLOBAL  WHERE " . 
                  "(PROGRAM_ID=" . $prog_id . ")";
  $result=mysql_query($query, $link);
  if ($result)
  {
    $row=mysql_fetch_array($result);
    $num=$row['COUNT(DISTINCT ENVIRONMENT_ID)'];
    mysql_free_result($result);
  }

  return $num;
}

/************************************************************************************************/
function get_inum_compilers($prog_id, $link)
{
  $num=0;

  $query="SELECT COUNT(DISTINCT COMPILER_ID) FROM STATS_COMP_GLOBAL_FLAGS  WHERE " . 
                  "(PROGRAM_ID=" . $prog_id . ")";
  $result=mysql_query($query, $link);
  if ($result)
  {
    $row=mysql_fetch_array($result);
    $num=$row['COUNT(DISTINCT COMPILER_ID)'];
    mysql_free_result($result);
  }

  return $num;
}

/************************************************************************************************/
function get_inum_datasets($prog_id, $link)
{
  $num=0;

  $query="SELECT COUNT(DISTINCT DATASET_ID) FROM STATS_EXEC_GLOBAL  WHERE " . 
                  "(PROGRAM_ID=" . $prog_id . ")";
  $result=mysql_query($query, $link);
  if ($result)
  {
    $row=mysql_fetch_array($result);
    $num=$row['COUNT(DISTINCT DATASET_ID)'];
    mysql_free_result($result);
  }

  return $num;
}

/************************************************************************************************/
function get_prog_name($id, $link)
{
  $num="";

  $query="SELECT NAME FROM PROGRAMS WHERE (PROGRAM_ID=" . $id . ")";
  $result=mysql_query($query, $link);
  if ($result)
  {
    $row=mysql_fetch_array($result);
    $num=$row['NAME'];
    mysql_free_result($result);
  }

  return $num;
}

/************************************************************************************************/
function get_prog_version($id, $link)
{
  $num="";

  $query="SELECT VERSION FROM PROGRAMS WHERE (PROGRAM_ID=" . $id . ")";
  $result=mysql_query($query, $link);
  if ($result)
  {
    $row=mysql_fetch_array($result);
    $num=$row['VERSION'];
    mysql_free_result($result);
  }

  return $num;
}

/************************************************************************************************/
function get_prog_id_from_comp_id($comp_id, $link)
{
  $prog_id="";

  $query="SELECT PROGRAM_ID FROM STATS_COMP_GLOBAL_FLAGS WHERE (COMPILE_ID=" . $comp_id . ")";
  $result=mysql_query($query, $link);
  if ($result)
  {
    $row=mysql_fetch_array($result);
    $prog_id=$row['PROGRAM_ID'];
    mysql_free_result($result);
  }

  return $prog_id;
}

/************************************************************************************************/
function get_prog_name_short($id, $link)
{
  $num="";

  $query="SELECT NAMESHORT FROM PROGRAMS WHERE (PROGRAM_ID=" . $id . ")";
  $result=mysql_query($query, $link);
  if ($result)
  {
    $row=mysql_fetch_array($result);
    $num=$row['NAMESHORT'];
    mysql_free_result($result);
  }

  return $num;
}

/************************************************************************************************/
function get_platform_name($id, $link)
{
  $num="";

  $query="SELECT * FROM PLATFORMS WHERE (PLATFORM_ID=" . $id . ")";
  $result=mysql_query($query, $link);
  if ($result)
  {
    $row=mysql_fetch_array($result);
    $num=$row['CPU_NAME'];
    mysql_free_result($result);
  }

  return $num;
}

/************************************************************************************************/
function get_environment_name($id, $link)
{
  $num="";

  $query="SELECT * FROM ENVIRONMENTS WHERE (ENVIRONMENT_ID=" . $id . ")";
  $result=mysql_query($query, $link);
  if ($result)
  {
    $row=mysql_fetch_array($result);
    $num=$row['OS'];
    mysql_free_result($result);
  }

  return $num;
}

/************************************************************************************************/
function get_compiler_name($id, $link)
{
  $query="SELECT * FROM COMPILERS WHERE (COMPILER_ID=" . $id . ")";
  $result=mysql_query($query, $link);
  $row=mysql_fetch_array($result);
  if ($result)
  {
    $num=$row['NAME'];
    $num1=$row['VERSION'];
    mysql_free_result($result);
    return $num . " " . $num1;
  }
  else
    return "";
}


/************************************************************************************************/
function get_command_line($id, $link)
{
  $num="";

  $query="SELECT * FROM DATASETS WHERE (DATASET_ID=" . $id . ")";
  $result=mysql_query($query, $link);
  if ($result)
  {
    $row=mysql_fetch_array($result);
    $num=$row['RUN_COMMAND_LINE'];
    mysql_free_result($result);
  }

  return $num;
}

/************************************************************************************************/
function get_dataset_id($prog_id, $ds_num, $link)
{
  $num="";

  $query="SELECT DATASET_ID FROM DATASETS WHERE PROGRAM_ID=" . $prog_id .
          " AND RUN_COMMAND_LINE LIKE \"" . $ds_num . ")%\"";
  $result=mysql_query($query, $link);
  if ($result)
  {
    $row=mysql_fetch_array($result);
    $num=$row['DATASET_ID'];
    mysql_free_result($result);
  }

  return $num;
}

/************************************************************************************************/
function print_info_db_common($out, $link)
{
  $num_progs=get_num_program($link);
  $num_plats=get_num_platform($link);
  $num_plats_conf=get_num_platform_configurations($link);
  $num_envs=get_num_environment($link);
  $num_cmplrs=get_num_compiler($link);

  $num_all_ofg=get_num_opt_flags_global($link);
  $num_all_ofgp=get_num_opt_flags_platforms_global($link);
  $num_all_op=get_num_opt_passes($link);

  fwrite($out, "Common info:\n\n");
  fwrite($out, "  Platforms:                                    " . $num_plats . "\n");
  fwrite($out, "  Platform configurations:                      " . $num_plats_conf . "\n");
  fwrite($out, "  Environments:                                 " . $num_envs . "\n");
  fwrite($out, "  Compilers:                                    " . $num_cmplrs . "\n");
  fwrite($out, "  Programs:                                     " . $num_progs . "\n");

  fwrite($out, "  Global optimization combinations:             " . $num_all_ofg . "\n");
  fwrite($out, "  Global optimization combinations (platforms): " . $num_all_ofgp . "\n");
  fwrite($out, "  Combination of optimization passes:           " . $num_all_op . "\n");
}     

/************************************************************************************************/
function print_info_db_experiments($out, $link)
{
  $num_progs_d=get_num_program_distinct($link);
  $num_plats_d=get_num_platform_distinct($link);
  $num_plat_feat_d=get_num_platform_features_distinct($link);
  $num_envs_d=get_num_environment_distinct($link);
  $num_cmplrs_d=get_num_compiler_distinct($link);

  $num_execs=get_num_executions($link);
  $num_comps=get_num_compilations($link);
  $num_comps_d=get_num_compilations_distinct($link);

  $num_comps_ici=get_num_compilations_with_ici($link);
  $num_comps_feat=get_num_compilations_with_feat($link);

  $num_ds=get_num_datasets_distinct($link);
  $num_ofg=get_num_opt_flags_global_distinct($link);

  fwrite($out, "\nLocal info mainly:\n\n");
  fwrite($out, "  Distinct platforms:                                      " . $num_plats_d . "\n");
  fwrite($out, "  Distinct environments:                                   " . $num_envs_d . "\n");
  fwrite($out, "  Distinct compilers:                                      " . $num_cmplrs_d . "\n");
  fwrite($out, "  Distinct programs:                                       " . $num_progs_d . "\n");
  fwrite($out, "  Distinct datasets:                                       " . $num_ds . "\n");
  fwrite($out, "\n");
  fwrite($out, "  Total number of executions:                              " . $num_execs . "\n");
  fwrite($out, "  Total number of compilations:                            " . $num_comps . "\n");
  fwrite($out, "  Total number of compilations with ICI passes:            " . $num_comps_ici . "\n");
  fwrite($out, "  Total number of compilations with ICI features:          " . $num_comps_feat . "\n");
  fwrite($out, "\n");
  fwrite($out, "  Distinct number of compilations (BIN MD5):               " . $num_comps_d . "\n");
  fwrite($out, "  Distinct global optimization combinations:               " . $num_ofg . "\n");
  fwrite($out, "  Distinct number of platform features during compilation: " . $num_plat_feat_d . "\n");
}

/************************************************************************************************/
function print_info_datasets($out, $prog_id, $link, $link1)
{
  $query="SELECT DISTINCT DATASET_ID FROM STATS_EXEC_GLOBAL  WHERE " . 
                  "(PROGRAM_ID=" . $prog_id . ")";
  $result=mysql_query($query, $link);

  if ($result)
  {
    while ($row=mysql_fetch_assoc($result))  //get one row at a time
    {
      $id_ds=$row['DATASET_ID'];
      $cl=get_command_line($id_ds, $link1); 
      fwrite($out, " DATASET_ID = " . str_pad($id_ds,21," ",STR_PAD_LEFT) . " ;    RUN_COMMAND_LINE =  " . $cl . "\n");
    }
    fwrite($out, "\n");
    mysql_free_result($result);
  }
}

/************************************************************************************************/
//This function is DEPRECATED
//
//FGG: I have been trying to build Paretto-like distribution but my first implementation
//was too complex and not very correct
function get_opt_cases($prog_id, $plat_id, $env_id, $cmplr_id, $ds_id, $time_thr, $cor, $which_time, $link, $c_link, $file_out, $spdt, $dspd, $cases)
{
  //Saving all speedups in $file_out if $file_out != ""
  if ($file_out!="")
  {
    $fo=fopen($file_out, 'w') or die("Can't open stats file for writing in get_opt_case function");
    $fo_s=fopen($file_out . "_s", 'w') or die("Can't open stats file for writing in get_opt_case function");
    $fo_bs=fopen($file_out . "_bs", 'w') or die("Can't open stats file for writing in get_opt_case function");
    $fo_sbs=fopen($file_out . "_sbs", 'w') or die("Can't open stats file for writing in get_opt_case function");
    $fo_sbs_m=fopen($file_out . "_sbs_m", 'w') or die("Can't open stats file for writing in get_opt_case function");
  }

  if ($cor==1)
    $xcor="AND (STATS_EXEC_GLOBAL.OUTPUT_CORRECT=1)";
  else
    $xcor="";

  $query="SELECT STATS_EXEC_GLOBAL." . $which_time . ", STATS_EXEC_GLOBAL.COMPILE_ID, " .
          "STATS_EXEC_GLOBAL.RUN_ID_ASSOCIATE, STATS_EXEC_GLOBAL.COMPILE_ID, " .
          "STATS_EXEC_GLOBAL.RUN_ID, STATS_EXEC_GLOBAL.BIN_SIZE " .
	  "FROM STATS_EXEC_GLOBAL " .
          " WHERE (STATS_EXEC_GLOBAL.PROGRAM_ID=" . $prog_id . ")" .
	  " AND (STATS_EXEC_GLOBAL.PLATFORM_ID=" . $plat_id . ")" .
	  " AND (STATS_EXEC_GLOBAL.ENVIRONMENT_ID=" . $env_id . ")";
  if ($ds_id!="")
    $query.=" AND (STATS_EXEC_GLOBAL.DATASET_ID=" . $ds_id . ")";
  $query.=" AND (STATS_EXEC_GLOBAL." . $which_time . ">" . $time_thr . ")" .
	  " AND (STATS_EXEC_GLOBAL.COMPILER_ID=" . $cmplr_id . ") " .
	  $xcor;
  $result=mysql_query($query, $link);

  $cases_s=array();
  $cases_b=array();
  $cases_os=array();
  $cases_r=array();
  $cases_c=array();
  $cases_d=array();
  $tcases_s=array(); //temporal arrays
  $tcases_b=array();
  $tcases_os=array();
  $tcases_r=array();
  $tcases_c=array();

  $rmax="";
  $cmax="";
  $spdmax=1.0;
  $spdold=1.0;
  $bsmin=0;
  $dspdmax=0.0;

  $t_assoc=0.0;
  $bs_assoc=0;
  $r_id_assoc="";
  $c_id_assoc="";
  $opt_assoc="";

  while ($row=mysql_fetch_assoc($result))  //get one row at a time
  {
    $t=$row[$which_time];
    $ra=$row["RUN_ID_ASSOCIATE"];
    $r=$row["RUN_ID"];
    $c=$row["COMPILE_ID"];
    $bs=$row["BIN_SIZE"];

    //Cache ID ASSOCIATES
    if ($ra!=$r_id_assoc)
    {
      //Get time for the associate run
      $query1="SELECT STATS_EXEC_GLOBAL." . $which_time . 
              ", STATS_EXEC_GLOBAL.BIN_SIZE, STATS_EXEC_GLOBAL.COMPILE_ID FROM STATS_EXEC_GLOBAL " .
           "WHERE (STATS_EXEC_GLOBAL.RUN_ID=" . $ra . ")";
      $result1=mysql_query($query1, $link);
      $row1=mysql_fetch_assoc($result1);
      $t_assoc=$row1[$which_time];
      $bs_assoc=$row1["BIN_SIZE"];
      $c_id_assoc=$row1["COMPILE_ID"];

      $opt_id=get_opt_id($c, $link);
      $opt_assoc=get_opt_flags($opt_id, $cmplr_id, $c_link);

      if ($bsmin==0) $bsmin=$bs_assoc;

      $r_id_assoc=$ra;
    }

    if (($t_assoc!=0) && ($t!=0))
    {
      $spd=$t_assoc/$t;

      if ($file_out!="")
      {
        fwrite($fo, str_pad(sprintf("%.2f", $spd), 8, " ", STR_PAD_LEFT));
        fwrite($fo, str_pad(sprintf("%u", $bs_assoc), 8, " ", STR_PAD_LEFT) . " / ");
        fwrite($fo, str_pad(sprintf("%u", $bs), 8, " ", STR_PAD_LEFT));
	fwrite($fo, str_pad(sprintf("%.1f", $t_assoc), 8, " ", STR_PAD_LEFT) . " / ");
	fwrite($fo, str_pad(sprintf("%.1f", $t), 8, " ", STR_PAD_LEFT));
	fwrite($fo, str_pad($r, 21, " ", STR_PAD_LEFT));
	fwrite($fo, str_pad($r_id_assoc, 21, " ", STR_PAD_LEFT));
	fwrite($fo, str_pad($c, 21, " ", STR_PAD_LEFT) . "\n");
        
	fwrite($fo_s, sprintf("%.2f", $spd) . "\n");
	fwrite($fo_bs, $bs . "\n");
	fwrite($fo_sbs, $bs . ", " . sprintf("%.2f", $spd) . "\n");
      }

      //similar speedup - check then binsize and size of the flags
      $select_case=0;
      
      if (($spd>$spdt) && ($spd>($spdmax-$dspd)) && ($spd<($spdmax+$dspd)))
      {
        if ($bs<$bsmin) $select_case=1;
	else
	{
           $opt_id=get_opt_id($c, $link);
           $opt=get_opt_flags($opt_id, $cmplr_id, $c_link);
           if (strlen($opt)<strlen($opt_assoc)) $select_case=1;
	}
      }
      elseif (($spd>$spdt) && ($spd>($spdmax+$dspd)))
      {
        $select_case=1;
      }
      
      if ($select_case==1)
      {
        $opt_id=get_opt_id($c, $link);
        $opt=get_opt_flags($opt_id, $cmplr_id, $c_link);

        $spdold=$spdmax;
        $spdmax=$spd;
	$bsmin=$bs;
        $rmax=$r;
	$cmax=$c;

        if (sizeof($cases_s)==0)
	{
          //create arrays
          for ($i=0; $i<$cases; $i++)
	  {
            if ($i==0)
	    {
              $cases_s[$i]=1.0;
              $cases_b[$i]=$bs_assoc;
              $cases_os[$i]=strlen($opt_assoc);
              $cases_r[$i]=$r_id_assoc;
              $cases_c[$i]=$c_id_assoc;
	    }
            else if ($i==($cases-1))
	    {
              $cases_s[$i]=$spdmax;
              $cases_b[$i]=$bs;
              $cases_os[$i]=strlen($opt);
              $cases_r[$i]=$r;
              $cases_c[$i]=$c;
	    }
	    else
	    {
              $cases_s[$i]=0;
              $cases_b[$i]=0;
              $cases_os[$i]=0;
              $cases_r[$i]="";
              $cases_c[$i]="";
	    }
	  }
	}
	else
	{
          //redo arrays based on Pareto chart
          unset($tcases_s);
          unset($tcases_b);
          unset($tcases_os);
          unset($tcases_r);
          unset($tcases_c);

          $dd1spd=($spdmax-1.0)/$cases;
          $spdold=$cases_s[$cases-1];
          $spd1=1.0+$dd1spd;
	  
          for ($i=0; $i<$cases; $i++)
	  {
            $tcases_s[$i]=0;
	    $tcases_b[$i]=0;
	    $tcases_os[$i]=0;
	    $tcases_r[$i]="";
	    $tcases_c[$i]="";
	  }

          for ($i=0; $i<$cases; $i++)
	  {
            if ($spdmax=1)
              $j=$cases-1;
            else
              $j=round($i*((1.0-$spdold)/(1.0-$spdmax)));

            if (($tcases_s[$j]==0) || ($cases_s[$i]!=0))
            {
              $tcases_s[$j]=$cases_s[$i];
              $tcases_b[$j]=$cases_b[$i];
              $tcases_os[$j]=$cases_os[$i];
              $tcases_r[$j]=$cases_r[$i];
              $tcases_c[$j]=$cases_c[$i];
	    }
	  }

          $tcases_s[$cases-1]=$spdmax;
          $tcases_b[$cases-1]=$bs;
          $tcases_os[$cases-1]=strlen($opt);
          $tcases_r[$cases-1]=$r;
          $tcases_c[$cases-1]=$c;

          unset($cases_s);
          unset($cases_b);
          unset($cases_os);
          unset($cases_r);
          unset($cases_c);

          for ($i=0; $i<$cases; $i++)
	  {
            $cases_s[$i]=$tcases_s[$i];
            $cases_b[$i]=$tcases_b[$i];
            $cases_os[$i]=$tcases_os[$i];
            $cases_r[$i]=$tcases_r[$i];
            $cases_c[$i]=$tcases_c[$i];
  	  }
        }
      }
      
      if ($spd>$spdt)
      {
        //Check within current array
        if ($spdmax=1)
         $j=$cases-1;
        else
          $j=round(($cases-1)*((1.0-$spd)/(1.0-$spdmax)));
        $opt_id=get_opt_id($c, $link);
        $opt=get_opt_flags($opt_id, $cmplr_id, $c_link);

//echo "SPD=" . $spd . " BIN_SIZE=" . $bs . " OPT_SIZE=" . strlen($opt) . " J=" . $j . "\n";

        $select_case=0;
        if ($cases_s[$j]==0) $select_case=1;
        else
	{
          //Check if the new case is better
          if (($spd>(1.0+$dspd)) && ($spd>($cases_s[$j]-$dspd)) && ($spd<($cases_s[$j]+$dspd)))
          {
            if ($bs<$cases_b[$j]) $select_case=1;
//   	    else if (strlen($opt)<$cases_os[$j]) $select_case=1;
   	  }
        }

        if ($select_case==1) 
	{
          $cases_s[$j]=$spd;
          $cases_b[$j]=$bs;
          $cases_os[$j]=strlen($opt);
          $cases_r[$j]=$r;
          $cases_c[$j]=$c;
	}
      }
    }
  }

  mysql_free_result($result);

  if (sizeof($cases_s)>1)
  {
    unset($cases_d);

    for ($i=0; $i<$cases; $i++)
    {
      if ($cases_c[$i]!="")
      {
        $cases_d[$i]=get_opt_case_deviation($prog_id, $plat_id, $env_id, $ds_id, $cases_c[$i], $time_thr, $cor, $which_time, $link);
	fwrite($fo_sbs_m, $cases_b[$i] . ", " . sprintf("%.2f", $cases_s[$i]) . "\n");
      }
    }
  }

  $num["speedup"]=$cases_s;
  $num["delta"]=$cases_d;
  $num["binsize"]=$cases_b;
  $num["optsize"]=$cases_os;
  $num["run_id"]=$cases_r;
  $num["compile_id"]=$cases_c;

  if ($file_out!="")
  {
    fclose($fo);
    fclose($fo_s);
    fclose($fo_bs);
    fclose($fo_sbs);
    fclose($fo_sbs_m);
  }

  return $num;
}

/************************************************************************************************/
function get_opt_cases_by_opt_id($prog_id, $plat_id, $env_id, $cmplr_id, $ds_id, $opt_id_to_find, $time_thr, $cor, $which_time, $link, $c_link, $pg_use)
{
  //Declare multi-objective speedup array
  //0 - execution time
  //1 - code size
  //2 - compilation time
  $speedup=array(0,1,2);

  //compile and run id
  $info=array(0,1,2,3);

  $xcor="";
  if ($cor==1)
    $xcor="AND (STATS_EXEC_GLOBAL.OUTPUT_CORRECT=1)";

  if ($pg_use==1)
    $xpg="AND (STATS_EXEC_GLOBAL.RUN_PG<>\"\")";
  else
    $xpg="AND (STATS_EXEC_GLOBAL.RUN_PG==\"\")";

  $query="SELECT STATS_EXEC_GLOBAL." . $which_time . ", STATS_EXEC_GLOBAL.COMPILE_ID, " .
          "STATS_EXEC_GLOBAL.RUN_ID_ASSOCIATE, STATS_EXEC_GLOBAL.COMPILE_ID, " .
          "STATS_EXEC_GLOBAL.RUN_ID, STATS_EXEC_GLOBAL.BIN_SIZE " .
          "FROM STATS_EXEC_GLOBAL " .
          " WHERE (STATS_EXEC_GLOBAL.PROGRAM_ID=" . $prog_id . ")" .
          " AND (STATS_EXEC_GLOBAL.PLATFORM_ID=" . $plat_id . ")" .
          " AND (STATS_EXEC_GLOBAL.ENVIRONMENT_ID=" . $env_id . ")" .
          " AND (STATS_EXEC_GLOBAL.DATASET_ID=" . $ds_id . ")" .
          " AND (STATS_EXEC_GLOBAL." . $which_time . ">" . $time_thr . ")" .
          " AND (STATS_EXEC_GLOBAL.COMPILER_ID=" . $cmplr_id . ") " .
          $xcor . " " . $xpg;
  $result=mysql_query($query, $link);

  if ($result)
  {
   $itx=0;
   $t_av=0;
   $ct_av=0;
   $ra_assoc=0;
   while ($row=mysql_fetch_assoc($result))  //get one row at a time
   {
     $t=$row[$which_time];
     $ra=$row["RUN_ID_ASSOCIATE"];
     $r=$row["RUN_ID"];
     $c=$row["COMPILE_ID"];
     $bs=$row["BIN_SIZE"];
     $opt_id=get_opt_id($c, $link);

     if ($opt_id==$opt_id_to_find)
     {
       $itx++;

       $ct=get_compilation_time($c, $link);

       $t_av+=$t;
       $ct_av+=$ct;

       $ra_assoc=$ra;
     }
   }

   mysql_free_result($result);

   $t=0;
   $ct=0;
   if ($itx!=0)
   {
     $t=$t_av/$itx;
     $ct=$ct_av/$itx;
   }

   //Get time for the associate run
   $query1="SELECT STATS_EXEC_GLOBAL." . $which_time . 
           ", STATS_EXEC_GLOBAL.BIN_SIZE, STATS_EXEC_GLOBAL.COMPILE_ID FROM STATS_EXEC_GLOBAL " .
        "WHERE (STATS_EXEC_GLOBAL.RUN_ID=" . $ra_assoc . ")";
   $result1=mysql_query($query1, $link);
   $row1=mysql_fetch_assoc($result1);
   if ($result1)
   {
    $result1=mysql_query($query1, $link);
    mysql_free_result($result1);

    $it=1;
    $t_assoc=$row1[$which_time];
    $bs_assoc=$row1["BIN_SIZE"];
    $c_id_assoc=$row1["COMPILE_ID"];
    $ct_assoc=get_compilation_time($c_id_assoc, $link);
    //Average for now
    $query2="SELECT STATS_EXEC_GLOBAL." . $which_time . ", STATS_EXEC_GLOBAL.COMPILE_ID, STATS_EXEC_GLOBAL.RANK, " .
        "STATS_EXEC_GLOBAL.RUN_ID_ASSOCIATE, STATS_EXEC_GLOBAL.COMPILE_ID, " .
        "STATS_EXEC_GLOBAL.RUN_ID, STATS_EXEC_GLOBAL.BIN_SIZE " .
        "FROM STATS_EXEC_GLOBAL " .
        " WHERE (STATS_EXEC_GLOBAL.PROGRAM_ID=" . $prog_id . ")" .
        " AND (STATS_EXEC_GLOBAL.PLATFORM_ID=" . $plat_id . ")" .
        " AND (STATS_EXEC_GLOBAL.ENVIRONMENT_ID=" . $env_id . ")" .
        " AND (STATS_EXEC_GLOBAL.DATASET_ID=" . $ds_id . ")" .
        " AND (STATS_EXEC_GLOBAL." . $which_time . ">" . $time_thr . ")" .
        " AND (STATS_EXEC_GLOBAL.COMPILER_ID=" . $cmplr_id . ") " .
        " AND (STATS_EXEC_GLOBAL.COMPILE_ID=" . $c_id_assoc . ")" .
        " AND (STATS_EXEC_GLOBAL.RUN_ID_ASSOCIATE=" . $ra . ")" .
        " AND (STATS_EXEC_GLOBAL.RUN_ID!=" . $ra . ")" .
        $xcor . " " . $xpg;
    $result2=mysql_query($query2, $link);

    if ($result2)
    {
      while ($row2=mysql_fetch_assoc($result2))  //get one row at a time
      {
        $it++;
        $t_assoc+=$row2[$which_time];
        $c_id_assoc=$row2["COMPILE_ID"];
        $rank=$row2["RANK"];
        $ct_assoc+=get_compilation_time($c_id_assoc, $link);
      }
      mysql_free_result($result2);

      $t_assoc=$t_assoc/$it;
      $ct_assoc=$ct_assoc/$it;

      $ct_assoc=get_compilation_time($c_id_assoc, $link);

      //Calculate speedups
      $spd=0;
      if (($t_assoc!=0) && ($t!=0)) $spd=$t_assoc/$t;
      $speedup[0]=$spd;

      $spd=0;
      if (($bs_assoc!=0) && ($bs!=0)) $spd=$bs_assoc/$bs;
      $speedup[1]=$spd;

      $spd=0;
      if (($ct_assoc!=0) && ($ct!=0)) $spd=$ct_assoc/$ct;
      $speedup[2]=$spd;

      $info[0]=$r;
      $info[1]=$c;
      $info[2]=$ra_assoc;
      $info[3]=$rank;
    }
   }
  }
  return array($speedup,$info);
}

/************************************************************************************************/
//FGG cleaned up and rewrote the whole get_opt_cases with averaging (2010.02.18)
function get_opt_cases1($prog_id, $plat_id, $env_id, $cmplr_id, $ds_id, $time_thr, $cor, $which_time, $link, $c_link, $file_out, $pg_use, $notes)
{
  //Declare multi-objective speedup array
  //0 - execution time
  //1 - code size
  //2 - compilation time
  $speedup=array(0,1,2);
  $speedup[0]=array();
  $speedup[1]=array();
  $speedup[2]=array();

  //compile and run id
  $info=array(0,1,2,3);
  $info[0]=array(); //run_id
  $info[1]=array(); //compile_id
  $info[2]=array(); //run_id_assoc
  $info[3]=array(); //rank

  //Saving all speedups in $file_out if $file_out != ""
  if ($file_out!="")
  {
    $fo=fopen($file_out, 'w') or die("Can't open stats file for writing in get_opt_case function");
    fwrite($fo, str_pad("SPD ET:", 9, " ", STR_PAD_LEFT));
    fwrite($fo, str_pad("SPD BS:", 9, " ", STR_PAD_LEFT));
    fwrite($fo, str_pad("SPD CT:", 9, " ", STR_PAD_LEFT));

    fwrite($fo, str_pad("ET_A:", 8, " ", STR_PAD_LEFT) . " / ");
    fwrite($fo, str_pad("ET:", 8, " ", STR_PAD_LEFT));

    fwrite($fo, str_pad("BS_A:", 8, " ", STR_PAD_LEFT) . " / ");
    fwrite($fo, str_pad("BS:", 8, " ", STR_PAD_LEFT));

    fwrite($fo, str_pad("CT_A:", 8, " ", STR_PAD_LEFT) . " / ");
    fwrite($fo, str_pad("CT:", 8, " ", STR_PAD_LEFT));

    fwrite($fo, str_pad("RUN_ID:", 21, " ", STR_PAD_LEFT));
    fwrite($fo, str_pad("RUN_ID_ASSOC:", 21, " ", STR_PAD_LEFT));
    fwrite($fo, str_pad("COMPILE_ID:", 21, " ", STR_PAD_LEFT));

    fwrite($fo, "\n");
  }

  $xcor="";
  if ($cor==1)
    $xcor="AND (STATS_EXEC_GLOBAL.OUTPUT_CORRECT=1)";

  $xnotes="";
  if ($notes!="")
    $xnotes="AND (STATS_EXEC_GLOBAL.NOTES LIKE \"" . $notes . ")\"";

  if ($pg_use==1)
    $xpg="AND (STATS_EXEC_GLOBAL.RUN_PG<>\"\")";
  else
    $xpg="AND (STATS_EXEC_GLOBAL.RUN_PG LIKE \"\")";

  #First find speedups, only then build area frontier
  $query="SELECT STATS_EXEC_GLOBAL." . $which_time . ", STATS_EXEC_GLOBAL.COMPILE_ID, STATS_EXEC_GLOBAL.RANK, " .
          "STATS_EXEC_GLOBAL.RUN_ID_ASSOCIATE, STATS_EXEC_GLOBAL.RUN_PG, STATS_EXEC_GLOBAL.COMPILE_ID, " .
          "STATS_EXEC_GLOBAL.RUN_ID, STATS_EXEC_GLOBAL.BIN_SIZE " .
          "FROM STATS_EXEC_GLOBAL " .
          " WHERE (STATS_EXEC_GLOBAL.PROGRAM_ID=" . $prog_id . ")" .
          " AND (STATS_EXEC_GLOBAL.PLATFORM_ID=" . $plat_id . ")" .
          " AND (STATS_EXEC_GLOBAL.ENVIRONMENT_ID=" . $env_id . ")" .
          " AND (STATS_EXEC_GLOBAL.DATASET_ID=" . $ds_id . ")" .
          " AND (STATS_EXEC_GLOBAL." . $which_time . ">" . $time_thr . ")" .
          " AND (STATS_EXEC_GLOBAL.COMPILER_ID=" . $cmplr_id . ") " .
          $xcor . " " . $xnotes . " " . $xpg;
  $result=mysql_query($query, $link);

  $bsmin=0;

  $t_assoc=0.0;
  $bs_assoc=0;
  $r_id_assoc="";
  $c_id_assoc="";
  $opt_assoc="";

  $v=0;

  //temporally averaging results
  $t_av=0;   $ct_av=0;   $ra_av=0;  $bs_av=0; $rank_av=""; $r_av=0;  $c_av=0;  $it_av=0;

  if ($result)
  {
    $finish=false;
    while (!$finish)  //get one row at a time
    {
      $row=mysql_fetch_assoc($result);

      if (!$row) $finish=true;
      else
      {
        $t=$row[$which_time];
        $ra=$row["RUN_ID_ASSOCIATE"];
        $r=$row["RUN_ID"];
        $rank=$row["RANK"];
        $c=$row["COMPILE_ID"];
        $bs=$row["BIN_SIZE"];
        $pg=$row["RUN_PG"];
        $ct=get_compilation_time($c, $link);
      }

      //Find info for RUN_IT_ASSOCIATE (baseline run) and remember it (cache)
      if ($ra!=$r_id_assoc)
      {
        //Get time for the associate run
        $query1="SELECT STATS_EXEC_GLOBAL." . $which_time . 
                ", STATS_EXEC_GLOBAL.BIN_SIZE, STATS_EXEC_GLOBAL.COMPILE_ID FROM STATS_EXEC_GLOBAL " .
                "WHERE (STATS_EXEC_GLOBAL.RUN_ID=" . $ra . ")";
        $result1=mysql_query($query1, $link);
        if ($result1)
        {
          $row1=mysql_fetch_assoc($result1);
          $result1=mysql_query($query1, $link);
          mysql_free_result($result1);

          $it=1;
          $t_assoc=$row1[$which_time];
          $bs_assoc=$row1["BIN_SIZE"];
          $c_id_assoc=$row1["COMPILE_ID"];
          $ct_assoc=get_compilation_time($c_id_assoc, $link);
          //Average for now
          $query2="SELECT STATS_EXEC_GLOBAL." . $which_time . ", STATS_EXEC_GLOBAL.COMPILE_ID, " .
              "STATS_EXEC_GLOBAL.RUN_ID_ASSOCIATE, STATS_EXEC_GLOBAL.COMPILE_ID, " .
              "STATS_EXEC_GLOBAL.RUN_ID, STATS_EXEC_GLOBAL.BIN_SIZE " .
              "FROM STATS_EXEC_GLOBAL " .
              " WHERE (STATS_EXEC_GLOBAL.PROGRAM_ID=" . $prog_id . ")" .
              " AND (STATS_EXEC_GLOBAL.PLATFORM_ID=" . $plat_id . ")" .
              " AND (STATS_EXEC_GLOBAL.ENVIRONMENT_ID=" . $env_id . ")" .
              " AND (STATS_EXEC_GLOBAL.DATASET_ID=" . $ds_id . ")" .
              " AND (STATS_EXEC_GLOBAL." . $which_time . ">" . $time_thr . ")" .
              " AND (STATS_EXEC_GLOBAL.COMPILER_ID=" . $cmplr_id . ") " .
              " AND (STATS_EXEC_GLOBAL.COMPILE_ID=" . $c_id_assoc . ")" .
              " AND (STATS_EXEC_GLOBAL.RUN_ID_ASSOCIATE=" . $ra . ")" .
              " AND (STATS_EXEC_GLOBAL.RUN_ID!=" . $ra . ")" .
              $xcor . " " . $xpg;
          $result2=mysql_query($query2, $link);
          if ($result2)
          {
            while ($row2=mysql_fetch_assoc($result2))  //get one row at a time
            {
              $it++;
              $t_assoc+=$row2[$which_time];
              $c_id_assoc=$row2["COMPILE_ID"];
              $ct_assoc+=get_compilation_time($c_id_assoc, $link);
            }
            mysql_free_result($result2);
          }

          $t_assoc=$t_assoc/$it;
          $ct_assoc=$ct_assoc/$it;

          $opt_id=get_opt_id($c, $link);
          $opt_assoc=get_opt_flags($opt_id, $cmplr_id, $c_link);

          if ($bsmin==0) $bsmin=$bs_assoc;

          $r_id_assoc=$ra;
        }
      }

      if (($ra!=$ra_av) || ($c!=$c_av) || $finish)
      {
        if ($ra_av!=0 || $finish)
        {
          //save previous speedup and null
          $xt=$t_av/$it_av;
          $xct=$ct_av/$it_av;

          $spd0=0;
          if (($t_assoc!=0) && ($xt!=0)) $spd0=$t_assoc/$xt;
          $speedup[0][$v]=$spd0;

          $spd1=0;
          if (($bs_assoc!=0) && ($bs!=0)) $spd1=$bs_assoc/$bs_av;
          $speedup[1][$v]=$spd1;

          $spd2=0;
          if (($ct_assoc!=0) && ($xct!=0)) $spd2=$ct_assoc/$xct;
          $speedup[2][$v]=$spd2;

          $info[0][$v]=$r_av;
          $info[1][$v]=$c_av;
          $info[2][$v]=$ra_av;
          $info[3][$v]=$rank_av;

          if ($file_out!="")
          {
            fwrite($fo, str_pad(sprintf("%2.2f", $spd0), 9, " ", STR_PAD_LEFT));
            fwrite($fo, str_pad(sprintf("%2.2f", $spd1), 9, " ", STR_PAD_LEFT));
            fwrite($fo, str_pad(sprintf("%2.2f", $spd2), 9, " ", STR_PAD_LEFT));

            fwrite($fo, str_pad(sprintf("%.1f", $t_assoc), 8, " ", STR_PAD_LEFT) . " / ");
            fwrite($fo, str_pad(sprintf("%.1f", $xt), 8, " ", STR_PAD_LEFT));

            fwrite($fo, str_pad(sprintf("%u", $bs_assoc), 8, " ", STR_PAD_LEFT) . " / ");
            fwrite($fo, str_pad(sprintf("%u", $bs_av), 8, " ", STR_PAD_LEFT));

            fwrite($fo, str_pad(sprintf("%.1f", $ct_assoc), 8, " ", STR_PAD_LEFT) . " / ");
            fwrite($fo, str_pad(sprintf("%.1f", $xct), 8, " ", STR_PAD_LEFT));

            fwrite($fo, str_pad($r_av, 21, " ", STR_PAD_LEFT));
            fwrite($fo, str_pad($r_id_assoc, 21, " ", STR_PAD_LEFT));
            fwrite($fo, str_pad($c_av, 21, " ", STR_PAD_LEFT) . "\n");
          }

          $v++;

          $t_av=0;   $ct_av=0;  $it_av=0;
        }
        $ra_av=$ra;
        $r_av=$r;
        $rank_av=$rank;
        $c_av=$c;
        $bs_av=$bs;
      }

      $it_av++;
      $t_av+=$t;
      $ct_av+=$ct;
    }

    mysql_free_result($result);
  }

  return array($speedup, $info);
}

/************************************************************************************************/
function sort_speedups($speedup, $info, $sort)
{
  $ospeedup=array(0,1,2);
  $ospeedup[0]=array();
  $ospeedup[1]=array();
  $ospeedup[2]=array();

  //compile and run id
  $oinfo=array(0,1,2,3);
  $oinfo[0]=array();
  $oinfo[1]=array();
  $oinfo[2]=array();
  $oinfo[3]=array();

  //copy arrays
  for ($i=0; $i<sizeof($speedup[0]); $i++)
  {
    $ospeedup[0][$i]=$speedup[0][$i];
    $ospeedup[1][$i]=$speedup[1][$i];
    $ospeedup[2][$i]=$speedup[2][$i];
    $oinfo[0][$i]=$info[0][$i];
    $oinfo[1][$i]=$info[1][$i];
    $oinfo[2][$i]=$info[2][$i];
    $oinfo[3][$i]=$info[3][$i];
  }

  // Sort array
  // Find which order
  // 0 - execution time speedup
  // 1 - code size improvement
  // 2 - compilation time speedup
  $srt=array(0,1,2);
  for ($i=0; $i<strlen($sort); $i++)
  {
   $x=substr($sort, $i, 1);
   if ($x=="0") $srt[$i]=0;
   else if ($x=="1") $srt[$i]=1;
   else if ($x=="2") $srt[$i]=2;
  }

  array_multisort($ospeedup[$srt[0]], SORT_DESC, SORT_NUMERIC,
                  $ospeedup[$srt[1]], SORT_DESC, SORT_NUMERIC,
                  $ospeedup[$srt[2]], SORT_DESC, SORT_NUMERIC,
                  $oinfo[0], $oinfo[1], $oinfo[2], $oinfo[3]);

  return array($ospeedup, $oinfo);
}

/************************************************************************************************/
function find_frontier($speedup, $info, $dimension, $cut)
{
  $ospeedup=array(0,1,2);
  $ospeedup[0]=array();
  $ospeedup[1]=array();
  $ospeedup[2]=array();

  //compile and run id
  $oinfo=array(0,1,2,3);
  $oinfo[0]=array();
  $oinfo[1]=array();
  $oinfo[2]=array();
  $oinfo[3]=array();

  $v=sizeof($speedup[0]);

  //Search optimal solutions
  if (sizeof($dimension)>0)
  {
    $v1=0;
    for ($i=0; $i<$v; $i++)
    {
      $optimal=true;
      for ($j=0; $j<$v; $j++)
      {
        if ($i!=$j)
        {
          $found=true;
          for ($k=0; $k<sizeof($dimension); $k++)
          {
            if ($speedup[$dimension[$k]][$i]>$speedup[$dimension[$k]][$j])
              $found=false;
          }
          if ($found)
          {
            $optimal=false;
            break;
          }
        }
      }

      if ($optimal)
      {
        $found=true;
        for ($k=0; $k<sizeof($cut); $k++)
        {
          if ($speedup[$k][$i]<$cut[$k])
            $found=false;
        }
        if ($found)
        {
          $ospeedup[0][$v1]=$speedup[0][$i];
          $ospeedup[1][$v1]=$speedup[1][$i];
          $ospeedup[2][$v1]=$speedup[2][$i];
          $oinfo[0][$v1]=$info[0][$i];
          $oinfo[1][$v1]=$info[1][$i];
          $oinfo[2][$v1]=$info[2][$i];
          $oinfo[3][$v1]=$info[3][$i];
          $v1++;
        }
      }
    }
  }
  else
  {
    #just check cut
    $v1=0;
    for ($i=0; $i<$v; $i++)
    {
      $found=true;
      for ($k=0; $k<sizeof($cut); $k++)
      {
        if ($speedup[$k][$i]<$cut[$k])
          $found=false;
      }
      if ($found)
      {
        $ospeedup[0][$v1]=$speedup[0][$i];
        $ospeedup[1][$v1]=$speedup[1][$i];
        $ospeedup[2][$v1]=$speedup[2][$i];
        $oinfo[0][$v1]=$info[0][$i];
        $oinfo[1][$v1]=$info[1][$i];
        $oinfo[2][$v1]=$info[2][$i];
        $oinfo[3][$v1]=$info[3][$i];
        $v1++;
      }
    }
  }

  return array($ospeedup, $oinfo);
}

/************************************************************************************************/
//we do not use it at the moment - should be cleaned up
function get_opt_case_deviation($prog_id, $plat_id, $env_id, $ds_id, $c_id, $time_thr, $cor, $which_time, $link)
{
  if ($cor==1)
    $xcor="AND (STATS_EXEC_GLOBAL.OUTPUT_CORRECT=1)";
  else
    $xcor="";

  $spdmin=0.0;
  $spdmax=0.0;

  $t_assoc=0.0;
  $r_id_assoc=0;

  $query="SELECT STATS_EXEC_GLOBAL." . $which_time . ", STATS_EXEC_GLOBAL.RUN_ID_ASSOCIATE, " .
             " STATS_EXEC_GLOBAL.RUN_ID " .
             " FROM STATS_EXEC_GLOBAL " .
             "WHERE (STATS_EXEC_GLOBAL.PROGRAM_ID=" . $prog_id . ")" .
  	     "AND (STATS_EXEC_GLOBAL.PLATFORM_ID=" . $plat_id . ")" .
	     "AND (STATS_EXEC_GLOBAL.ENVIRONMENT_ID=" . $env_id . ")" .
	     "AND (STATS_EXEC_GLOBAL.DATASET_ID=" . $ds_id . ")" .
	     "AND (STATS_EXEC_GLOBAL." . $which_time . ">" . $time_thr . ")" .
	     "AND (STATS_EXEC_GLOBAL.COMPILE_ID=" . $c_id . ")" .
	     $xcor;
  $result=mysql_query($query, $link);

  if ($result)
  {
    while ($row=mysql_fetch_assoc($result))  //get one row at a time
    {
      $t=$row[$which_time];
      $ra=$row["RUN_ID_ASSOCIATE"];
      $r=$row["RUN_ID"];

      if ($ra!=$r_id_assoc)
      {
        //Get time for the associate run
        $query1="SELECT STATS_EXEC_GLOBAL." . $which_time . " FROM STATS_EXEC_GLOBAL " .
             "WHERE (STATS_EXEC_GLOBAL.RUN_ID=" . $ra . ")";
        $result1=mysql_query($query1, $link);
        if ($result1)
        {
          $row1=mysql_fetch_assoc($result1);
          $t_assoc=$row1[$which_time];

          $r_id_assoc=$ra;

          mysql_free_result($result1);
        }

        if (($t_assoc!=0) && ($t!=0))
        {
          $spd=$t_assoc/$t;
          if ($spdmin==0.0) $spdmin=$spd;
          if ($spdmax==0.0) $spdmax=$spd;

          if ($spd<$spdmin)
            $spdmin=$spd;
          if ($spd>$spdmax)
            $spdmax=$spd;
        }
      }
    }

    mysql_free_result($result);
  }

  $delta=$spdmax-$spdmin;

  return $delta;
}

/************************************************************************************************/
function get_opt_id($c_id, $link)
{
  $opt_id="";

  if ($c_id!="")
  {
    $query="SELECT STATS_COMP_GLOBAL_FLAGS.OPT_ID FROM STATS_COMP_GLOBAL_FLAGS " .
             "WHERE (STATS_COMP_GLOBAL_FLAGS.COMPILE_ID=" . $c_id . ")";
    $result=mysql_query($query, $link);

    if ($result)
    {
      if (($row=mysql_fetch_assoc($result))!=null)
        $opt_id=$row["OPT_ID"];

      mysql_free_result($result);
    }
  }

  return $opt_id;
}

/************************************************************************************************/
function get_opt_platform_feature_id($c_id, $link)
{
  $opt_id="";

  if ($c_id!="")
  {
    $query="SELECT STATS_COMP_GLOBAL_FLAGS.PLATFORM_FEATURE_ID FROM STATS_COMP_GLOBAL_FLAGS " .
             "WHERE (STATS_COMP_GLOBAL_FLAGS.COMPILE_ID=" . $c_id . ")";
    $result=mysql_query($query, $link);
    if ($result)
    {
      if (($row=mysql_fetch_assoc($result))!=null)
        $opt_id=$row["PLATFORM_FEATURE_ID"];

      mysql_free_result($result);
    }
  }

  return $opt_id;
}

/************************************************************************************************/
function get_opt_platform_id($p_id, $link)
{
  $opt_id="";

  if ($p_id!=0)
  {
    $query="SELECT PLATFORM_FEATURES.OPT_PLATFORM_ID FROM PLATFORM_FEATURES " .
             "WHERE (PLATFORM_FEATURE_ID=" . $p_id . ")";
    $result=mysql_query($query, $link);
    if ($result)
    {
      if (($row=mysql_fetch_assoc($result))!=null)
        $opt_id=$row["OPT_PLATFORM_ID"];

      mysql_free_result($result);
    }
  }

  return $opt_id;
}

/************************************************************************************************/
function get_opt_flags($opt_id, $cmplr_id, $link)
{
  $opt_flags="";

  if ($opt_id!="")
  {
    $query="SELECT OPT_FLAGS FROM OPT_FLAGS_GLOBAL " .
             "WHERE (OPT_ID=" . $opt_id . ")" .
             "AND (COMPILER_ID=" . $cmplr_id . ")";
    $result=mysql_query($query, $link);
    if ($result)
    {
      if (($row=mysql_fetch_assoc($result))!=null)
        $opt_flags=$row["OPT_FLAGS"];

      mysql_free_result($result);
    }
  }

  return $opt_flags;
}

/************************************************************************************************/
function get_opt_passes($comp_id, $cmplr_id, $link, $c_link)
{
  $ret=array();

  $query="SELECT FUNCTION_NAME, OPT_PASSES_ID " .
             " FROM PROGRAM_PASSES " .
             " WHERE (COMPILE_ID=" . $comp_id . ")";
  $result=mysql_query($query, $link);
  if ($result)
  {
    while ($row=mysql_fetch_assoc($result))  //get one row at a time
    {
      $func=$row["FUNCTION_NAME"];
      if ($func=="") $func="ALL";
      $y=$row["OPT_PASSES_ID"];

      $query1="SELECT PASSES " .
                 " FROM OPT_PASSES " .
                 " WHERE (OPT_PASSES_ID=" . $y . ")" .
                 " AND (COMPILER_ID=" . $cmplr_id . ")";
      $result1=mysql_query($query1, $c_link);
      if ($result1)
      {
        if (($row1=mysql_fetch_assoc($result1))!=null)
          $ret[$func]=$row1["PASSES"];
        mysql_free_result($result1);
      }
    }

    mysql_free_result($result);
  }

  return $ret;
}

/************************************************************************************************/
function get_static_program_features($c, $link)
{
  $ret=array();

  $query="SELECT FUNCTION_NAME, PASS, STATIC_FEATURE_VECTOR " .
             " FROM PROGRAM_FEATURES " .
             " WHERE (COMPILE_ID=" . $c . ")";
  $result=mysql_query($query, $link);
  if ($result)
  {
    while ($row=mysql_fetch_assoc($result))  //get one row at a time
    {
      $func=$row["FUNCTION_NAME"];
      $pass=$row["PASS"];
      $sfv=$row["STATIC_FEATURE_VECTOR"];

      $ret[$func]=$pass . ";" . $sfv;
    }

    mysql_free_result($result);
  }

  return $ret; 
}


/************************************************************************************************/
function get_opt_flags_platform($opt_id, $cmplr_id, $link)
{
  $opt_flags="";

  if ($opt_id!="")
  {
    $query="SELECT OPT_FLAGS FROM OPT_FLAGS_GLOBAL_PLATFORM " .
             "WHERE (OPT_PLATFORM_ID=" . $opt_id . ")" .
             "AND (COMPILER_ID=" . $cmplr_id . ")";
    $result=mysql_query($query, $link);
    if ($result)
    {
      if (($row=mysql_fetch_assoc($result))!=null)
        $opt_flags=$row["OPT_FLAGS"];

      mysql_free_result($result);
    }
  }

  return $opt_flags;
}

/************************************************************************************************/
function get_arch_cfg($plat_feat_id, $link)
{
  $ret="";
  if ($plat_feat_id!="")
  {
    $query="SELECT ARCH_CFG FROM PLATFORM_FEATURES " .
              "WHERE (PLATFORM_FEATURE_ID=" . $plat_feat_id . ")";
    $result=mysql_query($query, $link);
    if ($result)
    {
      if (($row=mysql_fetch_assoc($result))!=null)
        $ret=$row["ARCH_CFG"];

      mysql_free_result($result);
    }
  }

  return $ret;
}

/************************************************************************************************/
function get_arch_size($plat_feat_id, $link)
{
  $ret="";
  if ($plat_feat_id!="")
  {
    $query="SELECT ARCH_SIZE FROM PLATFORM_FEATURES " .
              "WHERE (PLATFORM_FEATURE_ID=" . $plat_feat_id . ")";
    $result=mysql_query($query, $link);
    if ($result)
    {
      if (($row=mysql_fetch_assoc($result))!=null)
        $ret=$row["ARCH_SIZE"];

      mysql_free_result($result);
    }
  }

  return $ret;
}

/************************************************************************************************/
function get_run_id_associate_total($prog_id, $plat_id, $env_id, $cmplr_id, $ds_id, $link)
{
  $num=0;

  $query="SELECT DISTINCT STATS_EXEC_GLOBAL.RUN_ID_ASSOCIATE FROM STATS_EXEC_GLOBAL " .
           "WHERE (STATS_EXEC_GLOBAL.PROGRAM_ID=" . $prog_id . ")" .
           "AND (STATS_EXEC_GLOBAL.PLATFORM_ID=" . $plat_id . ")" .
           "AND (STATS_EXEC_GLOBAL.ENVIRONMENT_ID=" . $env_id . ")" .
           "AND (STATS_EXEC_GLOBAL.DATASET_ID=" . $ds_id . ")" .
           "AND (STATS_EXEC_GLOBAL.COMPILER_ID=" . $cmplr_id . ") ";
  $result=mysql_query($query, $link);
  if ($result)
  {
    while ($row=mysql_fetch_assoc($result))
      $num++;

    mysql_free_result($result);
  }

  return $num;
}

/************************************************************************************************/
function get_run_id_associate_cor($prog_id, $plat_id, $env_id, $cmplr_id, $ds_id, $time_thr, $which_time, $link)
{
  $num=0;

  $query="SELECT DISTINCT STATS_EXEC_GLOBAL.RUN_ID_ASSOCIATE FROM STATS_EXEC_GLOBAL " .
           "WHERE (STATS_EXEC_GLOBAL.PROGRAM_ID=" . $prog_id . ")" .
           "AND (STATS_EXEC_GLOBAL.PLATFORM_ID=" . $plat_id . ")" .
           "AND (STATS_EXEC_GLOBAL.ENVIRONMENT_ID=" . $env_id . ")" .
           "AND (STATS_EXEC_GLOBAL.DATASET_ID=" . $ds_id . ")" .
           "AND (" . $which_time . ">" . $time_thr . ")" .
           "AND (STATS_EXEC_GLOBAL.COMPILER_ID=" . $cmplr_id . ") ";
  $result=mysql_query($query, $link);
  if ($result)
  {
    while ($row=mysql_fetch_assoc($result))  //get one row at a time
      $num++;

    mysql_free_result($result);
  }

  return $num;
}

/************************************************************************************************/
function get_time_range($prog_id, $plat_id, $env_id, $cmplr_id, $ds_id, $time_thr, $which_time, $link)
{
  $tmin=0.0;
  $tmax=0.0;

  $query="SELECT STATS_EXEC_GLOBAL." . $which_time . ", STATS_EXEC_GLOBAL.COMPILE_ID FROM STATS_EXEC_GLOBAL " .
           "WHERE (STATS_EXEC_GLOBAL.PROGRAM_ID=" . $prog_id . ")" .
           " AND (STATS_EXEC_GLOBAL.DATASET_ID=" . $ds_id . ")" .
           " AND (STATS_EXEC_GLOBAL.PLATFORM_ID=" . $plat_id . ")" .
           " AND (STATS_EXEC_GLOBAL.ENVIRONMENT_ID=" . $env_id . ")" .
           " AND (STATS_EXEC_GLOBAL.COMPILER_ID=" . $cmplr_id . ") " .
           " AND (" . $which_time . ">" . $time_thr . ")" ;

  $result=mysql_query($query, $link);
  if ($result)
  {
    $tmin=-1;
    $tmax=-1;

    while ($row=mysql_fetch_assoc($result))  //get one row at a time
    {
      $t=$row[$which_time];
      if ($tmin==-1 || $t<$tmin) $tmin=$t;
      if ($tmax==-1 || $t>$tmax) $tmax=$t;
    }
    mysql_free_result($result);
  }

  if ($tmin==-1) $tmin=0;
  if ($tmax==-1) $tmax=0;

  $num=array($tmin, $tmax);

  return $num;
}

/************************************************************************************************/
function get_compile_time_range($prog_id, $plat_id, $env_id, $cmplr_id, $time_thr, $link)
{
  $tmin=0.0;
  $tmax=0.0;

  $query="SELECT STATS_COMP_GLOBAL_FLAGS.COMPILE_TIME FROM STATS_COMP_GLOBAL_FLAGS " .
           "WHERE (STATS_COMP_GLOBAL_FLAGS.PROGRAM_ID=" . $prog_id . ")" .
           "  AND (STATS_COMP_GLOBAL_FLAGS.PLATFORM_ID=" . $plat_id . ")" .
           "  AND (STATS_COMP_GLOBAL_FLAGS.ENVIRONMENT_ID=" . $env_id . ")" .
           "  AND (STATS_COMP_GLOBAL_FLAGS.COMPILER_ID=" . $cmplr_id . ") " .
           "  AND (STATS_COMP_GLOBAL_FLAGS.COMPILE_TIME > " . $time_thr . ")" ;

  $result=mysql_query($query, $link);
  if ($result)
  {
    $tmin=-1;
    $tmax=-1;
    while ($row=mysql_fetch_assoc($result))  //get one row at a time
    {
      $t=$row["COMPILE_TIME"];
      if ($tmin==-1 || $t<$tmin) $tmin=$t;
      if ($tmax==-1 || $t>$tmax) $tmax=$t;
    }
    mysql_free_result($result);
  }
  
  if ($tmin==-1) $tmin=0;
  if ($tmax==-1) $tmax=0;

  $num=array($tmin, $tmax);

  return $num;
}

/************************************************************************************************/
function get_bin_size_range($prog_id, $plat_id, $env_id, $cmplr_id, $ds_id, $link)
{
  $bs_min=0.0;
  $bs_max=0.0;

  $query="SELECT STATS_EXEC_GLOBAL.BIN_SIZE FROM STATS_EXEC_GLOBAL " .
           "WHERE (STATS_EXEC_GLOBAL.PROGRAM_ID=" . $prog_id . ")" .
           " AND (STATS_EXEC_GLOBAL.DATASET_ID=" . $ds_id . ")" .
           " AND (STATS_EXEC_GLOBAL.PLATFORM_ID=" . $plat_id . ")" .
           " AND (STATS_EXEC_GLOBAL.ENVIRONMENT_ID=" . $env_id . ")" .
           " AND (STATS_EXEC_GLOBAL.COMPILER_ID=" . $cmplr_id . ") " .
           " AND (STATS_EXEC_GLOBAL.BIN_SIZE>0)";

  $result=mysql_query($query, $link);
  if ($result)
  {
    $bs_min=-1;
    $bs_max=-1;
    while ($row=mysql_fetch_assoc($result))  //get one row at a time
    {
      $bs=$row["BIN_SIZE"];
      if ($bs_min==-1 || $bs<$bs_min) $bs_min=$bs;
      if ($bs_max==-1 || $bs>$bs_max) $bs_max=$bs;
    }
    mysql_free_result($result);
  }

  if ($bs_min==-1) $bs_min=0;
  if ($bs_max==-1) $bs_max=0;

  $bs=array($bs_min, $bs_max);

  return $bs;
}

/************************************************************************************************/
function append_to_file($fo, $fn)
{
  $fi=fopen($fn, "r");

  fwrite($fo, str_pad("SPEEDUP:", 8, " ", STR_PAD_LEFT));
  fwrite($fo, str_pad("TIME:", 8, " ", STR_PAD_LEFT));
  fwrite($fo, str_pad("TIME_B:", 8, " ", STR_PAD_LEFT));
  fwrite($fo, str_pad("RUN_ID:", 21, " ", STR_PAD_LEFT));
  fwrite($fo, str_pad("RUN_ID_ASSOC:", 21, " ", STR_PAD_LEFT));
  fwrite($fo, str_pad("COMPILE_ID:", 21, " ", STR_PAD_LEFT) . "\n");

  while (!feof($fi)) 
  {
    $str=fgets($fi);
    fwrite($fo, $str);
  }

  fclose($fi);
}

/************************************************************************************************/
function copy2ctdb_run($run_id, $link, $link_out, $debug)
{
  $success=false;

  $query="SELECT * FROM STATS_EXEC_GLOBAL " .
           "WHERE (STATS_EXEC_GLOBAL.RUN_ID=" . $run_id . ")";
  $result=mysql_query($query, $link);
  if ($result)
  {
    $run_id_assoc="";
    $comp_id="";
    $ds_id="";

    if ($row=mysql_fetch_assoc($result))
    {
      $i=0;
      foreach ($row as $cell)
      {
        $field=mysql_fetch_field($result, $i);
        $fn=$field->name;

        if ($fn=="RUN_ID_ASSOCIATE")
          $run_id_assoc=$cell;
        else if ($fn=="COMPILE_ID")
          $comp_id=$cell;
        else if ($fn=="DATASET_ID")
          $ds_id=$cell;
        $i++;
      }

      mysql_free_result($result);

      if ($debug)  echo "Coping RUN INFO (RUN_ID=" . $run_id . ") ...\n";
        if (!copy2ctdb_simple("STATS_EXEC_GLOBAL", "RUN_ID", $run_id, $link, $link_out, $debug)) return false;

      if ($debug)  echo "Coping RUN ASSOCIATE INFO (RUN_ID=" . $run_id_assoc . ") ...\n";
        copy2ctdb_simple("STATS_EXEC_GLOBAL", "RUN_ID", $run_id_assoc, $link, $link_out, $debug);

      if ($debug)  echo "Coping COMPILE INFO (COMPILE_ID=" . $comp_id . ") ...\n";
        copy2ctdb_simple("STATS_COMP_GLOBAL_FLAGS", "COMPILE_ID", $comp_id, $link, $link_out, $debug);

      if ($debug)  echo "Coping PROGRAM PASSES INFO (COMPILE_ID=" . $comp_id . ") ...\n";
        copy2ctdb_simple("PROGRAM_PASSES", "COMPILE_ID", $comp_id, $link, $link_out, $debug);

      if ($debug)  echo "Coping PROGRAM FEATURES INFO (COMPILE_ID=" . $comp_id . ") ...\n";
        copy2ctdb_simple("PROGRAM_FEATURES", "COMPILE_ID", $comp_id, $link, $link_out, $debug);

//      if ($debug)  echo "Coping DATASET INFO (DATASET_ID=" . $ds_id . ") ...\n";
//        copy2ctdb_simple("DATASETS", "DATASET_ID", $ds_id, $link, $link_out, $debug);

      $success=true;
    }
  }

  return $success;
}

/************************************************************************************************/
//Need to be cleaned up!..
function copy2ctdb_simple($table, $field, $id, $link, $link_out, $debug)
{
  $success=false;

  $query="SELECT * FROM " . $table . 
           " WHERE (" . $table . "." . $field . "=" . $id . ")";
  $result=mysql_query($query, $link_out);
  if ($result)
  {
    if ($row=mysql_fetch_assoc($result))
    {
      mysql_free_result($result);
      //found already in the output database
      //do nothing for now (maybe later increment counter)
      if ($debug)
        echo "Found the same entry in the collective database...\n";
    }
    else
    {
      mysql_free_result($result);

      $query="SELECT * FROM " . $table . 
               " WHERE (" . $table . "." . $field . "=" . $id . ")";
      $result=mysql_query($query, $link);
      if ($result)
      {
        while ($row=mysql_fetch_assoc($result))
        {
          $query1="INSERT INTO " . $table . " (";
          $query1d="";
          $i=0;
          foreach ($row as $cell)
          {
            $field=mysql_fetch_field($result, $i);
            $fn=$field->name;

            if ($i!=0)
            {
              $query1.=",";
              $query1d.=",";
            }
            $query1.= $fn;
            $query1d.="'" . $cell . "'";

            $i++;
          }

          $query1.=") VALUE (" . $query1d . ")";
          $result1=mysql_query($query1, $link_out);
        }
        $success=true;

        mysql_free_result($result);
      }
    }
  }

  return $success;
}

/************************************************************************************************/
function get_db_version($link)
{
  $version="";

  $query="SELECT VERSION FROM INFORMATION";
  $result=mysql_query($query, $link);
  if ($result)
  {
    if ($row=mysql_fetch_assoc($result))
      $version=$row["VERSION"];

    mysql_free_result($result);
  }

  return $version;
}

/************************************************************************************************/
function get_db_local_version($file)
{
  $version1=0;
  $version2=0;

  $fn=fopen($file, 'r');

  if ($fn)
  {
    $version1=fgets($fn);
    $version2=fgets($fn);

    fclose($fn);
  }

  $ver=array($version1, $version2);

  return $ver;
}

/************************************************************************************************/
function ccc_mysql_select_db($database, $link, $file_db_ver)
{
 mysql_select_db($database, $link) or die( "Error: Unable to select database\n");

 $ver=intval(get_db_version($link));
 $ver1=get_db_local_version($file_db_ver);
 $ver1a=intval($ver1[0]); $ver1b=intval($ver1[1]);

 if (($ver<$ver1a) || ($ver>$ver1b))
 {
   echo "\nError: This CCC version is not intended to work with the version of the CCC database:\n";
   echo "CCC can work with CCC databases: " . $ver1a . " .. " . $ver1b . "\n";
   echo "CCC database version:            " . $ver . "\n";
   die ("\n");
 }
}

/************************************************************************************************/
function makeColorFromImpr($d)
{
  $clr="00";
  if      (( 5.0 < $d) && ($d < 10.0)) $clr="30";
  elseif ((10.0 < $d) && ($d < 15.0)) $clr="60";
  elseif ((15.0 < $d) && ($d < 20.0)) $clr="90";
  elseif ((20.0 < $d) && ($d < 25.0)) $clr="C0";
  elseif ((25.0 < $d)) $clr="FF"; 
  return $clr;  
}

/************************************************************************************************/
function makeColorFromImpr9($d)
{
  $clr="000000";

  if                     ($d < 0.4)  $clr="0000C0";
  elseif ((0.4 <= $d) && ($d < 0.6))  $clr="000090";
  elseif ((0.6 <= $d) && ($d < 0.8)) $clr="000060";
  elseif ((0.8 <= $d) && ($d < 1))   $clr="000030";
  elseif       ($d==1.0)             $clr="000000";
  elseif ((1.0 < $d) && ($d < 1.2))  $clr="300000";
  elseif ((1.2 <= $d) && ($d < 1.5))  $clr="600000";
  elseif ((1.5 <= $d) && ($d < 2.0))  $clr="900000";
  elseif ((2.0 <= $d) && ($d < 2.5))  $clr="C00000";
  elseif  (2.5 <= $d)                 $clr="FF0000"; 

  return $clr;
}

/************************************************************************************************/
function  getDB_ID($dbn, $adb_n)
{
  $ret=-1;
  $i=0;
  foreach ($adb_n as $key => $value)
  {
    if ($value==$dbn)
    {
      $ret=$i;
      break;
    }
    $i++;
  }
  return $ret;
}

/************************************************************************************************/
function make_seed()
{
  list($usec, $sec) = explode(' ', microtime());
  return (float) $sec + ((float) $usec * 100000);
}

/************************************************************************************************/
function returnRandStr()
{
  srand(make_seed());

  $ret="";
  for ($i=0; $i<20; $i++)
    $ret.=chr(rand(0,9)+48);


  return $ret;
}

/************************************************************************************************/
function makeColorFromImpr1($dist, $dist_min, $dist_max)
{
  $clr="FFFFFF";

  if ($dist<=1)
  {
    $q=55+(($dist-$dist_min)/(1-$dist_min))*200;
    $c=dechex($q);
    if ($q<16) $c="0".$c;
    $clr="FF".$c.$c;
  }

  if ($dist>=1)
  {
    $q=55+(1-(($dist-1)/($dist_max-1)))*200;
    $c=dechex($q);
    if ($q<16) $c="0".$c;
    $clr=$c.$c."FF";
  }

  return $clr;  
}


/************************************************************************************************/
function check_opt_cases($prog_id, $plat_id, $env_id, $cmplr_id, $link)
{
  $ret=0;

  $query="SELECT STATS_EXEC_GLOBAL.RUN_TIME FROM STATS_EXEC_GLOBAL " .
           "WHERE (STATS_EXEC_GLOBAL.PROGRAM_ID=" . $prog_id . ")" .
           "AND (STATS_EXEC_GLOBAL.PLATFORM_ID=" . $plat_id . ")" .
           "AND (STATS_EXEC_GLOBAL.ENVIRONMENT_ID=" . $env_id . ")" .
           "AND (STATS_EXEC_GLOBAL.COMPILER_ID=" . $cmplr_id . ") " .
           "AND (STATS_EXEC_GLOBAL.BIN_SIZE>0) " .
           "AND (STATS_EXEC_GLOBAL.RUN_TIME>0) ";
  $result=mysql_query($query, $link);
  if ($result)
  {
    while ($row=mysql_fetch_assoc($result))
      $ret++;

    mysql_free_result($result);
  }

  return $ret;
}

/************************************************************************************************/
function get_time_by_run_id($r_id, $link)
{
  $t=0.0;
  $tu=0.0;

  $query="SELECT RUN_TIME, RUN_TIME_USER FROM STATS_EXEC_GLOBAL " .
           "WHERE (STATS_EXEC_GLOBAL.RUN_ID=" . $r_id . ")";

  $result=mysql_query($query, $link);
  if ($result)
  {
    if ($row=mysql_fetch_assoc($result))  //get one row at a time
    {
      $t=$row["RUN_TIME"];
      $tu=$row["RUN_TIME_USER"];
    }

    mysql_free_result($result);
  }

  return array($t, $tu);
}

/************************************************************************************************/
function get_compilation_time($c_id, $link)
{
  $t=0.0;

  $query="SELECT COMPILE_TIME FROM STATS_COMP_GLOBAL_FLAGS " .
           "WHERE (STATS_COMP_GLOBAL_FLAGS.COMPILE_ID=" . $c_id . ")";

  $result=mysql_query($query, $link);
  if ($result)
  {
    if ($row=mysql_fetch_assoc($result))  //get one row at a time
      $t=$row["COMPILE_TIME"];
    mysql_free_result($result);
  }

  return $t;
}

/************************************************************************************************/
//Print to screen and to file 
function ccc_print($fo, $print)
{
  echo $print;
  if ($fo)
    fwrite($fo, $print);
}

/************************************************************************************************/
//Get all distinct datasets
function get_datasets($prog_id, $plat_id, $env_id, $cmplr_id, $link)
{
  $datasets=array();

  #Check datasets
  $query1="SELECT DISTINCT DATASET_ID " .
          " FROM STATS_EXEC_GLOBAL" .
          " WHERE (STATS_EXEC_GLOBAL.PROGRAM_ID=" . $prog_id . ")" .
          " AND (STATS_EXEC_GLOBAL.PLATFORM_ID=" . $plat_id . ")" .
          " AND (STATS_EXEC_GLOBAL.ENVIRONMENT_ID=" . $env_id . ")" .
          " AND (STATS_EXEC_GLOBAL.COMPILER_ID=" . $cmplr_id . ") ";

  $result1=mysql_query($query1, $link);
  if ($result1)
  {
    $i=0;
    while ($row1=mysql_fetch_assoc($result1))
      $datasets[$i++]=$row1["DATASET_ID"];

    mysql_free_result($result1);
  }


  return $datasets;
}

/************************************************************************************************/
//Get comp_id by run_id
function get_comp_id_by_run_id($run_id, $link)
{
  $comp_id="";

  $query1="SELECT COMPILE_ID " .
          " FROM STATS_EXEC_GLOBAL" .
          " WHERE (STATS_EXEC_GLOBAL.RUN_ID=" . $run_id . ")";

  $result1=mysql_query($query1, $link);
  if ($result1)
  {
    $row1=mysql_fetch_assoc($result1);
    $comp_id=$row1["COMPILE_ID"];

    mysql_free_result($result1);
  }
  return $comp_id;
}

/************************************************************************************************/
//Get compiler_id by run_id
function get_compiler_id_by_run_id($run_id, $link)
{
  $cmplr_id="";

  $query1="SELECT COMPILER_ID " .
          " FROM STATS_EXEC_GLOBAL" .
          " WHERE (STATS_EXEC_GLOBAL.RUN_ID=" . $run_id . ")";

  $result1=mysql_query($query1, $link);
  if ($result1)
  {
    $row1=mysql_fetch_assoc($result1);
    $cmplr_id=$row1["COMPILER_ID"];

    mysql_free_result($result1);
  }
  return $cmplr_id;
}

/************************************************************************************************/
//Get program_id by run_id
function get_program_id_by_run_id($run_id, $link)
{
  $prog_id="";

  $query1="SELECT PROGRAM_ID " .
          " FROM STATS_EXEC_GLOBAL" .
          " WHERE (STATS_EXEC_GLOBAL.RUN_ID=" . $run_id . ")";

  $result1=mysql_query($query1, $link);
  if ($result1)
  {
    $row1=mysql_fetch_assoc($result1);
    $prog_id=$row1["PROGRAM_ID"];

    mysql_free_result($result1);
  }
  return $prog_id;
}

/************************************************************************************************/
//Delete file tree
function delTree($dir) 
{
  $files = glob( $dir . '*', GLOB_MARK );
  foreach( $files as $file )
  {
    if( substr( $file, -1 ) == '/' )
      delTree( $file );
    else
      unlink( $file );
  }

  if (is_dir($dir)) rmdir( $dir );
}

?>
