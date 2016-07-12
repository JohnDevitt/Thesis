# ICI plugin building makefile

# Author: Grigori Fursin (http://fursin.net/research)
#
# (C) 2007-2010, UNIDAPT Group, INRIA, France (http://unidapt.org)

PLUGIN_NAME=save-executed-passes-with-time
INSTALL_PLUGIN_NAME=$(ICI_PLUGIN_VER)-$(PLUGIN_NAME)

# Edit the makevars below to match your individual setup.

# Compiler
# Any compiler that generates an ABI compatible with your platform is OK.
# In particular, any GCC can be used to compile the plugin as long as the
# correct headers are used (cf. PLUGIN_INCPATH).
CC_SHARED=gcc -fPIC
LD_SHARED=gcc -shared

# Plugin include files are available here:
PLUGIN_INCPATH=../../include
EXTRACINCLUDE=

# Installation directory for the shared library
INSTALL_DIR=../../install/lib

# Unless your plugin consists of multiple modules, no further editing is necessary...
#
all: $(INSTALL_PLUGIN_NAME).so

$(INSTALL_PLUGIN_NAME).so: $(PLUGIN_NAME).c
	$(CC_SHARED) -c $< -g -I$(PLUGIN_INCPATH) $(EXTRAINCLUDE)
	$(LD_SHARED) -o $@ $(<:.c=.o)

install:
	[ -d $(INSTALL_DIR) ] || mkdir -p $(INSTALL_DIR)
	cp -f $(INSTALL_PLUGIN_NAME).so $(INSTALL_DIR)

clean:
	rm -f $(INSTALL_PLUGIN_NAME).o
	rm -f $(INSTALL_PLUGIN_NAME).so
