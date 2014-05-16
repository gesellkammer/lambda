librevel_a_SOURCES = avilib.c avilib.h \
		BaseEncoder.cpp BaseEncoder.h \
		revelcore.cpp \
		XvidEncoder.cpp XvidEncoder.h

AM_CFLAGS := $(AM_CFLAGS) -I../include
INCLUDES = $(AM_CFLAGS)
lib_LIBRARIES = librevel.a
include_HEADERS = ../include/revel.h

reveltest_SOURCES = reveltest.cpp
reveltest_LDFLAGS = -L../lib -L.
reveltest_LDADD = -lrevel -lxvidcore
reveltest.cpp: chimes.raw
chimes.raw:
	cp ../chimes.raw .

check_PROGRAMS = reveltest
TESTS = $(check_PROGRAMS)
CLEANFILES = checkers.avi chimes.raw