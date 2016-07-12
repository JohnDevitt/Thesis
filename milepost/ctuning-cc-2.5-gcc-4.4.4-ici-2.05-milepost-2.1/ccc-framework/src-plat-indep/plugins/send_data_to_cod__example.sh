#!/bin/bash

# Copyright (C) 2004-2009 by Grigori G.Fursin
#
# http://fursin.net/research
# 
# UNIDAPT Group
# http://unidapt.org

export CCC_CTS_URL=cTuning.org/wiki/index.php/Special:CDatabase?request=
export CCC_CTS_USER=
export CCC_CTS_PASS=
export CCC_CTS_DB=cod_opt_cases_test
 
export CCC_FILE=send_data_to_cod__example.txt
export CCC_WEB_SERVICE=add_opt_case
php send_data_to_cod.php
