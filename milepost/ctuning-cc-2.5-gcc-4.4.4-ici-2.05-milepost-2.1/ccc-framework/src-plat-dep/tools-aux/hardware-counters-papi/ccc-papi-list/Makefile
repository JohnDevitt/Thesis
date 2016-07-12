# Declarations
MISCOPT =	
MISCOPTLINK =   -lm -lpapi
COMPILER =	gcc
EXTRAINCLUDE =

TARGET =        ccc-papi-list

DEST_DEF = ../../../install/bin
ifneq ($(DEST),)
DEST_DEF=$(DEST)
endif

# Actions
all:		
		rm -f $(TARGET)
		$(COMPILER) $(MISCOPT) $(MISCOPTLINK) $(EXTRAINCLUDE) -o $(TARGET) *.c

install:
	        if test ! -d "$(DEST)/bin" ; then mkdir $(DEST)/bin; fi
	        cp -f $(TARGET) $(DEST)/bin

clean:		
		rm -f core 
		rm -f *.o
		rm -f *.a
		rm -f $(TARGET)
