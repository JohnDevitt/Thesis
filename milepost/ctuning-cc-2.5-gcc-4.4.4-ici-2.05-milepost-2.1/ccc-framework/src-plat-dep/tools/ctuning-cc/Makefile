# Author: Grigori Fursin (http://fursin.net/research)
#
# (C) 2010, University of Versailles at Saint-Quentin-en-Yvelines, France
# (C) 2004-2009, UNIDAPT Group, INRIA, France

# Declarations
MISCOPT =
MISCOPTLINK =   -lm
COMPILER =      gcc
INCLUDE =       -I../../include
EXTRAINCLUDE =

TARGET =        ctuning-cc
TARGET1 =       ctuning-c++
TARGET2 =       ctuning-fortran

DEST =          ../../../install

# Actions
all:		
		$(COMPILER) $(MISCOPT) $(INCLUDE) $(EXTRAINCLUDE) -c ../../lib/futils.c
		$(COMPILER) $(MISCOPT) $(INCLUDE) $(EXTRAINCLUDE) -c ../../lib/fuuid.c
		$(COMPILER) $(MISCOPT) $(MISCOPTLINK) $(INCLUDE) $(EXTRAINCLUDE) -o $(TARGET) *.c futils.o fuuid.o
		$(COMPILER) $(MISCOPT) $(MISCOPTLINK) $(INCLUDE) $(EXTRAINCLUDE) -o $(TARGET1) *.c futils.o fuuid.o
		$(COMPILER) $(MISCOPT) $(MISCOPTLINK) $(INCLUDE) $(EXTRAINCLUDE) -o $(TARGET2) *.c futils.o fuuid.o

install:
	        if test ! -d "$(DEST)/bin" ; then mkdir $(DEST)/bin; fi
	        mv -f $(TARGET) $(DEST)/bin
	        mv -f $(TARGET1) $(DEST)/bin
	        mv -f $(TARGET2) $(DEST)/bin
		rm -f *.o

clean:		
		rm -f core 
		rm -f *.o
		rm -f *.a
		rm -f $(TARGET)
		rm -f $(TARGET1)
		rm -f $(TARGET2)
