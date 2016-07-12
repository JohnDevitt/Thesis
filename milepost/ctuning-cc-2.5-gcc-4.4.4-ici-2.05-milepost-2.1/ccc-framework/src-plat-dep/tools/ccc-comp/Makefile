# Declarations
MISCOPT =	
MISCOPTLINK =   -lm
COMPILER =	gcc
INCLUDE =       -I../../include
EXTRAINCLUDE =

TARGET =        ccc-comp
TARGET1 =       ccc-comp1

DEST =          ../../../install

# Actions
all:		
		$(COMPILER) $(MISCOPT) $(INCLUDE) $(EXTRAINCLUDE) -c ../../lib/futils.c
		$(COMPILER) $(MISCOPT) $(INCLUDE) $(EXTRAINCLUDE) -c ../../lib/fuuid.c
		$(COMPILER) $(MISCOPT) $(MISCOPTLINK) $(INCLUDE) $(EXTRAINCLUDE) -o $(TARGET1) *.c futils.o fuuid.o

install:
	        if test ! -d "$(DEST)/bin" ; then mkdir $(DEST)/bin; fi
	        cp -f $(TARGET) $(DEST)/bin
	        cp -f $(TARGET1) $(DEST)/bin

clean:		
		rm -f core 
		rm -f *.o
		rm -f *.a
		rm -f $(TARGET1)
