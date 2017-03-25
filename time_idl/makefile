CC            = /usr/bin/g++
CPPFLAGS      = -g -c
LDFLAGS       = -g
OMNI_HOME     = /home/patrick/Downloads/omniORB-4.2.1
OMNI_INCLUDES = -I$(OMNI_HOME)/include
OMNI_LIB_DIR  = $(OMNI_HOME)/lib
OMNIIDL       = $(OMNI_HOME)/bin/omniidl
INCLUDES      = $(OMNI_INCLUDES)
LIBS          = -lomniORB4 -lomnithread -lomniDynamic4
SERVER_OBJECTS= timeSK.o \
                myserver.o

CLIENT_OBJECTS=timeSK.o\
               myclient.o

all: server client

client: $(CLIENT_OBJECTS)
	$(CC) $(LDFLAGS) -o myclient -L$(OMNI_HOME)/lib $(CLIENT_OBJECTS) $(LIBPATH) $(LIBS)
                                                                          
server: $(SERVER_OBJECTS)
	$(CC) $(LDFLAGS) -o myserver -L$(OMNI_HOME)/lib $(SERVER_OBJECTS) $(LIBPATH) $(LIBS)
                                                                                
timeSK.o: timeSK.cc
	$(CC) $(CPPFLAGS) $(INCLUDES) timeSK.cc

myserver.o:
	$(CC) $(CPPFLAGS) $(INCLUDES) myserver.cc

myclient.o:
	$(CC) $(CPPFLAGS) $(INCLUDES) myclient.cc

timeSK.cc:
	$(OMNI_HOME)/bin/omniidl -bcxx time.idl
                                                                                
clean clean_all:
	rm -fr *.o
	rm -fr *.hh
	rm -fr *SK.cc
	rm -fr myserver
	rm -fr myclient

