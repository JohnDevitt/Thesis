/*
 * Copyright (C) 2004-2009 by Grigori Fursin
 *
 * http://fursin.net/research
 *
 * UNIDAPT Group
 * http://unidapt.org
 */

#ifndef fuuid_INCLUDED
#define fuuid_INCLUDED

int get_uuid(unsigned long* rnd1, unsigned long* rnd2);
int get_filename_uuid(char* str, char* name);

#endif
