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
 
export CCC_FILE=send_data_to_cod__example1.txt
export CCC_WEB_SERVICE=add_opt_case
php send_data_to_cod.php
