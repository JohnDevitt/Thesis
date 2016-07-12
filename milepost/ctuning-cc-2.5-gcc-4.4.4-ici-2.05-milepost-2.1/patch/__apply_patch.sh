#!/bin/bash

# Copyright (C) 2007-2010 by Grigori Fursin
#
# http://fursin.net/research
# 
# UNIDAPT Group
# http://unidapt.org

cd gcc-4.4.4
patch --no-backup-if-mismatch -p1 < ../patch-gcc-4.4.3-ici-2.05-milepost-2.0
