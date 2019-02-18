ACLOCAL_AMFLAGS = -I build-aux/m4

SUBDIRS = univalue

UNIVALUE_CFLAGS = -I$(top_srcdir)/univalue/include

include_HEADERS = include/rlpvalue.h
noinst_HEADERS = lib/rlpvalue_utffilter.h

lib_LTLIBRARIES = librlpvalue.la

pkgconfigdir = $(libdir)/pkgconfig
pkgconfig_DATA = pc/librlpvalue.pc

librlpvalue_la_SOURCES = \
	lib/rlpvalue.cpp \
	lib/rlpvalue_get.cpp \
	lib/rlpvalue_read.cpp \
	lib/rlpvalue_write.cpp

librlpvalue_la_LDFLAGS = \
	-version-info $(LIBRLPVALUE_CURRENT):$(LIBRLPVALUE_REVISION):$(LIBRLPVALUE_AGE) \
	-no-undefined
librlpvalue_la_CXXFLAGS = -I$(top_srcdir)/include

TESTS = test/object test/unitester

noinst_PROGRAMS = src/rlp $(TESTS)

src_rlp_SOURCES = \
	src/InfInt.h \
	src/tool.cpp \
	src/rlp2json.cpp \
	test/utilstrencodings.cpp
src_rlp_LDADD = librlpvalue.la univalue/.libs/libunivalue.a $(ARGP_LIBS)
src_rlp_CXXFLAGS = -I$(top_srcdir)/include $(UNIVALUE_CFLAGS)
src_rlp_LDFLAGS = -static $(LIBTOOL_APP_LDFLAGS)

TEST_DATA_DIR=test/data

test_unitester_SOURCES = \
	src/rlp2json.cpp \
	test/unitester.cpp \
	test/utilstrencodings.h test/utilstrencodings.cpp
test_unitester_LDADD = librlpvalue.la univalue/.libs/libunivalue.a
test_unitester_CXXFLAGS = -I$(top_srcdir)/include $(UNIVALUE_CFLAGS) \
	-DJSON_TEST_SRC=\"$(srcdir)/$(TEST_DATA_DIR)\"
test_unitester_LDFLAGS = -static $(LIBTOOL_APP_LDFLAGS)

test_object_SOURCES = test/object.cpp
test_object_LDADD = librlpvalue.la
test_object_CXXFLAGS = -I$(top_srcdir)/include
test_object_LDFLAGS = -static $(LIBTOOL_APP_LDFLAGS)

TEST_FILES = \
	test/data/example.json \
	test/data/invalidRLPTest.json \
	test/data/longlist.json \
	test/data/rlptest.json

EXTRA_DIST=$(TEST_FILES)
