CC            = /usr/bin/g++
CPPFLAGS      = -g -c -Wall
LDFLAGS       = -g
OMNI_HOME     = /home/patrick/Downloads/omniORB-4.2.1
OMNI_INCLUDES = -I$(OMNI_HOME)/include
OMNI_LIB_DIR  = $(OMNI_HOME)/lib
OMNIIDL       = $(OMNI_HOME)/bin/omniidl
INCLUDES      = $(OMNI_INCLUDES)
LIBS          = -lomniORB4 -lomnithread -lomniDynamic4

CLIENT_OBJECTS=climateSK.o\
               client.o

climateSK.cc:
	$(OMNI_HOME)/bin/omniidl -bcxx climate.idl

climateSK.o: climateSK.cc
	$(CC) $(CPPFLAGS) $(INCLUDES) climateSK.cc

client: $(CLIENT_OBJECTS)
	$(CC) $(LDFLAGS) -o client -L$(OMNI_HOME)/lib $(CLIENT_OBJECTS) $(LIBPATH) $(LIBS)

client.o: climateSK.cc
	$(CC) $(CPPFLAGS) $(INCLUDES) client.cc
                                                                                
clean clean_all:
	rm -fr *.o
	rm -fr *.hh
	rm -fr *SK.cc
	rm -fr client

