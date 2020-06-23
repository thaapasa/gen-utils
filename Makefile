# Makefile for the general utility library
#
# Author: Tuukka Haapasalo
# Modified: 2010-11-30

# Preprocessor options
AR=ar
CC=g++
LD=g++
OPTFLAGS=-O3
# OPTFLAGS=-g 
INCLUDES=-Iinclude
CXXFLAGS=-Wall -Wno-deprecated
LDLIBS=-lm
LIBUTIL=dist/libutil.a

# Set to 'true' to build SSL support (SSLSocket)
BUILDSSL = false

SRCS = \
  src/main/cpp/String.cpp \
  src/main/cpp/Log.cpp\
  src/main/cpp/Random.cpp\
  src/main/cpp/File.cpp\
  src/main/cpp/FileNo.cpp\
  src/main/cpp/FileReader.cpp\
  src/main/cpp/FileWriter.cpp\
  src/main/cpp/Settings.cpp\
  src/main/cpp/ArgumentReader.cpp\
  src/main/cpp/NumberUtils.cpp\
  src/main/cpp/Timer.cpp\
  src/main/cpp/BitSet.cpp\
  src/main/cpp/Socket.cpp\
  src/main/cpp/Thread.cpp\
  src/main/cpp/BinaryData.cpp\
  src/main/cpp/Scheduler.cpp\
  src/main/cpp/Date.cpp\
  src/main/cpp/exception/Exception.cpp\
  src/main/cpp/exception/SocketException.cpp

TESTSRCS = \
  src/test/cpp/ArgumentReaderTest.cpp \
  src/test/cpp/RandomTest.cpp \
  src/test/cpp/MatrixTest.cpp \
  src/test/cpp/TimerTest.cpp \
  src/test/cpp/GeneralTest.cpp \
  src/test/cpp/FileTest.cpp \
  src/test/cpp/StringTest.cpp \
  src/test/cpp/SocketTest.cpp \
  src/test/cpp/SocketTest2.cpp \
  src/test/cpp/DeletorTest.cpp \
  src/test/cpp/ThreadTest.cpp \
  src/test/cpp/Vector2DTest.cpp \
  src/test/cpp/BinaryDataTest.cpp \
  src/test/cpp/LogTest.cpp \
  src/test/cpp/ArrayTest.cpp \
  src/test/cpp/BitSetTest.cpp
  
ifeq ($(BUILDSSL), true)
SRCS += src/main/cpp/SSLSocket.o
INCLUDE += -I$(OPENSSL_HOME)/include
endif


.PHONY: all
all: lib tests

include Makefile.system

OBJS = $(SRCS:.cpp=.o)

CXXFLAGS += $(OPTFLAGS) $(INCLUDES)

# Suffix triggers
#################

.SUFFIXES:
.SUFFIXES: .cpp .o .exe .test

.cpp.o:
	$(CC) $(CXXFLAGS) -c $< -o $(<:.cpp=.o)
.cpp.exe: $< $(LIBUTIL) $(OBJ)
	$(CC) $(CXXFLAGS) $< -Ldist -lutil $(LDPATH) $(LDLIBS) -o $(<:.cpp=.exe)
.cpp.test: $< $(LIBUTIL) $(OBJ)
	$(CC) $(CXXFLAGS) $< -Ldist -lutil $(LDPATH) $(LDLIBS) -o $(<:.cpp=.test)
 

.PHONY: lib
lib: $(LIBUTIL)


# The library file
##################

$(LIBUTIL): $(OBJS)
	$(AR) r $(LIBUTIL) $(OBJS)

# Tests
##################

.PHONY: tests
tests: $(LIBUTIL) $(TESTS)


.PHONY: runtests
runtests: tests
	for test in $(TESTS) ; do \
		./$${test} 2>&1; \
	done

.PHONY: grindtests
grindtests: tests
	for test in $(TESTS) ; do \
		valgrind ./$${test} ; \
	done

.PHONY: cleantests
cleantests:
	rm -f $(TESTS)

# Other targets
##################

.PHONY: clean
clean: cleantests
	echo Cleaning up...
	rm -f $(OBJS) $(TESTS) 

.PHONY: realclean
realclean: clean
	echo Cleaning up, deleting dependencies
	rm -f .depend

# Dependencies 
##################

.depend:
	echo Creating dependencies
	rm -f .depend
	touch .depend
	for f in $(SRCS); do \
		gcc $(CXXFLAGS) -MM -MT `echo $$f | sed s/[.]cpp/.o/` $$f >>.depend ; \
	done
	for f in $(TESTSRCS); do \
		gcc $(CXXFLAGS) -MM -MT `echo $$f | sed s/[.]cpp/.test/` $$f >>.depend ; \
	done

.PHONY: depend
depend: 
	echo Creating dependencies
	rm -f .depend
	touch .depend
	for f in $(SRCS); do \
		gcc $(CXXFLAGS) -MM -MT `echo $$f | sed s/[.]cpp/.o/` $$f >>.depend ; \
	done
	for f in $(TESTSRCS); do \
		gcc $(CXXFLAGS) -MM -MT `echo $$f | sed s/[.]cpp/.test/` $$f >>.depend ; \
	done

include .depend
