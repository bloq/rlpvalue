ACLOCAL_AMFLAGS = -I build-aux/m4

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

TESTS = test/object # test/unitester test/no_nul

noinst_PROGRAMS = $(TESTS) # test/test_json

TEST_DATA_DIR=test

# test_unitester_SOURCES = test/unitester.cpp
# test_unitester_LDADD = librlpvalue.la
# test_unitester_CXXFLAGS = -I$(top_srcdir)/include -DJSON_TEST_SRC=\"$(srcdir)/$(TEST_DATA_DIR)\"
# test_unitester_LDFLAGS = -static $(LIBTOOL_APP_LDFLAGS)

# test_test_json_SOURCES = test/test_json.cpp
# test_test_json_LDADD = librlpvalue.la
# test_test_json_CXXFLAGS = -I$(top_srcdir)/include
# test_test_json_LDFLAGS = -static $(LIBTOOL_APP_LDFLAGS)

# test_no_nul_SOURCES = test/no_nul.cpp
# test_no_nul_LDADD = librlpvalue.la
# test_no_nul_CXXFLAGS = -I$(top_srcdir)/include
# test_no_nul_LDFLAGS = -static $(LIBTOOL_APP_LDFLAGS)

test_object_SOURCES = test/object.cpp
test_object_LDADD = librlpvalue.la
test_object_CXXFLAGS = -I$(top_srcdir)/include
test_object_LDFLAGS = -static $(LIBTOOL_APP_LDFLAGS)

# EXTRA_DIST=$(TEST_FILES) $(GEN_SRCS)
