# Declarations
MISCOPT =	
MISCOPTLINK =   -lm
COMPILER =	gcc
INCLUDE =       -I../../../include
EXTRAINCLUDE =

TARGET =        ccc-ml-accumulate-features

DEST =          ../../../../install

# Actions
all:		
		$(COMPILER) $(MISCOPT) $(INCLUDE) $(EXTRAINCLUDE) -c ../../../lib/futils.c
		$(COMPILER) $(MISCOPT) $(INCLUDE) $(EXTRAINCLUDE) -c ../../../lib/fuuid.c
		$(COMPILER) $(MISCOPT) $(MISCOPTLINK) $(INCLUDE) $(EXTRAINCLUDE) -o $(TARGET) *.c futils.o fuuid.o

install:
	        if test ! -d "$(DEST)/bin" ; then mkdir $(DEST)/bin; fi
	        cp -f $(TARGET) $(DEST)/bin

clean:		
		rm -f core 
		rm -f *.o
		rm -f *.a
		rm -f $(TARGET)
