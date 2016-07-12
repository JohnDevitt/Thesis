#!/bin/bash

# Copyright (C) 2004-2009 by Grigori G.Fursin
#
# http://fursin.net/research
# 
# UNIDAPT Group
# http://unidapt.org

export CCC_CTS_URL=unidapt.org/intranet/wiki/index.php/Special:CDatabase?request=
export CCC_CTS_USER=Test
export CCC_CTS_PASS=LsH4rgB
export CCC_CTS_DB=cccdb_all
 
export CCC_FILE=send_data_to_cod__example2_comp_assoc.txt
export CCC_WEB_SERVICE=add_opt_case_comp
php send_data_to_cod.php

export CCC_FILE=send_data_to_cod__example2_run_assoc.txt
export CCC_WEB_SERVICE=add_opt_case_run
php send_data_to_cod.php

export CCC_FILE=send_data_to_cod__example2_comp.txt
export CCC_WEB_SERVICE=add_opt_case_comp
php send_data_to_cod.php

export CCC_FILE=send_data_to_cod__example2_run.txt
export CCC_WEB_SERVICE=add_opt_case_comp
php send_data_to_cod.php
