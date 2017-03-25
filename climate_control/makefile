CC            = /usr/bin/g++
CPPFLAGS      = -g -c
LDFLAGS       = -g
OMNI_HOME     = /home/patrick/Downloads/omniORB-4.2.1
OMNI_INCLUDES = -I$(OMNI_HOME)/include
OMNI_LIB_DIR  = $(OMNI_HOME)/lib
OMNIIDL       = $(OMNI_HOME)/bin/omniidl
INCLUDES      = $(OMNI_INCLUDES)
LIBS          = -lomniORB4 -lomnithread -lomniDynamic4

climateSK.cc:
	$(OMNI_HOME)/bin/omniidl -bcxx climate.idl
                                                                                
clean clean_all:
	rm -fr *.o
	rm -fr *.hh
	rm -fr *SK.cc

