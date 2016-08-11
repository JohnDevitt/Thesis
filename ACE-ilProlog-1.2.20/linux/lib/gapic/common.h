

#ifndef GAPIC_COMMON_INC
#define GAPIC_COMMON_INC

#ifndef cygwin
         
    #include <dlfcn.h>

    #include <link.h>

#endif

#define GAPIC_EXTERN_SO

#include <string.h>
#include <stdlib.h>
#include <stdio.h>
#include "gapic.h"


#ifdef _atom_array_INC
#error include common.h before atom_array.h
#endif

#ifdef _int_array_INC
#error include common.h before int_array.h
#endif

#ifdef _float_array_INC
#error include common.h before float_array.h
#endif

 

#ifndef GAPIC_MODULE_INIT_PROC
	#define GAPIC_MODULE_INIT_PROC gapic_module_init
#endif



#endif
