all: ml-feat-proc

# Installation directory for the shared library
INSTALL_DIR=../../../install/bin

XSB_DIR=${PWD}/../../../install/3.2
XSB_DIR_ADD=i686-pc-linux-gnu
XSB_DIR_CFG=${XSB_DIR}/config/${XSB_DIR_ADD}

ml-feat-proc: ml-feat-proc.c
	echo ${XSB_DIR}
	gcc -c ml-feat-proc.c -g -I${XSB_DIR}/emu -I${XSB_DIR_CFG}
	gcc -o ml-feat-proc ml-feat-proc.o ${XSB_DIR_CFG}/saved.o/xsb.o -lm -ldl -Wl,-export-dynamic -lpthread
	
install:
	mkdir -p            $(INSTALL_DIR)
	cp -f ml-feat-proc  $(INSTALL_DIR)
	cp -f featlstn*.P    $(INSTALL_DIR)

clean:
	rm -f ml-feat-proc
	rm -f GCC
	rm -f *.o
	rm -f *.so
