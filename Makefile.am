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

TESTS = test/object test/unitester test/no_nul

noinst_PROGRAMS = $(TESTS) test/test_json

TEST_DATA_DIR=test

test_unitester_SOURCES = test/unitester.cpp
test_unitester_LDADD = librlpvalue.la
test_unitester_CXXFLAGS = -I$(top_srcdir)/include -DJSON_TEST_SRC=\"$(srcdir)/$(TEST_DATA_DIR)\"
test_unitester_LDFLAGS = -static $(LIBTOOL_APP_LDFLAGS)

test_test_json_SOURCES = test/test_json.cpp
test_test_json_LDADD = librlpvalue.la
test_test_json_CXXFLAGS = -I$(top_srcdir)/include
test_test_json_LDFLAGS = -static $(LIBTOOL_APP_LDFLAGS)

test_no_nul_SOURCES = test/no_nul.cpp
test_no_nul_LDADD = librlpvalue.la
test_no_nul_CXXFLAGS = -I$(top_srcdir)/include
test_no_nul_LDFLAGS = -static $(LIBTOOL_APP_LDFLAGS)

test_object_SOURCES = test/object.cpp
test_object_LDADD = librlpvalue.la
test_object_CXXFLAGS = -I$(top_srcdir)/include
test_object_LDFLAGS = -static $(LIBTOOL_APP_LDFLAGS)

TEST_FILES = \
	$(TEST_DATA_DIR)/fail10.json \
	$(TEST_DATA_DIR)/fail11.json \
	$(TEST_DATA_DIR)/fail12.json \
	$(TEST_DATA_DIR)/fail13.json \
	$(TEST_DATA_DIR)/fail14.json \
	$(TEST_DATA_DIR)/fail15.json \
	$(TEST_DATA_DIR)/fail16.json \
	$(TEST_DATA_DIR)/fail17.json \
	$(TEST_DATA_DIR)/fail18.json \
	$(TEST_DATA_DIR)/fail19.json \
	$(TEST_DATA_DIR)/fail1.json \
	$(TEST_DATA_DIR)/fail20.json \
	$(TEST_DATA_DIR)/fail21.json \
	$(TEST_DATA_DIR)/fail22.json \
	$(TEST_DATA_DIR)/fail23.json \
	$(TEST_DATA_DIR)/fail24.json \
	$(TEST_DATA_DIR)/fail25.json \
	$(TEST_DATA_DIR)/fail26.json \
	$(TEST_DATA_DIR)/fail27.json \
	$(TEST_DATA_DIR)/fail28.json \
	$(TEST_DATA_DIR)/fail29.json \
	$(TEST_DATA_DIR)/fail2.json \
	$(TEST_DATA_DIR)/fail30.json \
	$(TEST_DATA_DIR)/fail31.json \
	$(TEST_DATA_DIR)/fail32.json \
	$(TEST_DATA_DIR)/fail33.json \
	$(TEST_DATA_DIR)/fail34.json \
	$(TEST_DATA_DIR)/fail35.json \
	$(TEST_DATA_DIR)/fail36.json \
	$(TEST_DATA_DIR)/fail37.json \
	$(TEST_DATA_DIR)/fail38.json \
	$(TEST_DATA_DIR)/fail39.json \
	$(TEST_DATA_DIR)/fail40.json \
	$(TEST_DATA_DIR)/fail41.json \
	$(TEST_DATA_DIR)/fail42.json \
	$(TEST_DATA_DIR)/fail44.json \
	$(TEST_DATA_DIR)/fail3.json \
	$(TEST_DATA_DIR)/fail4.json \
	$(TEST_DATA_DIR)/fail5.json \
	$(TEST_DATA_DIR)/fail6.json \
	$(TEST_DATA_DIR)/fail7.json \
	$(TEST_DATA_DIR)/fail8.json \
	$(TEST_DATA_DIR)/fail9.json \
	$(TEST_DATA_DIR)/pass1.json \
	$(TEST_DATA_DIR)/pass2.json \
	$(TEST_DATA_DIR)/pass3.json \
	$(TEST_DATA_DIR)/round1.json \
	$(TEST_DATA_DIR)/round2.json \
	$(TEST_DATA_DIR)/round3.json \
	$(TEST_DATA_DIR)/round4.json \
	$(TEST_DATA_DIR)/round5.json \
	$(TEST_DATA_DIR)/round6.json \
	$(TEST_DATA_DIR)/round7.json

EXTRA_DIST=$(TEST_FILES) $(GEN_SRCS)
