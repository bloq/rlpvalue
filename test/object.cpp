// Copyright (c) 2014 BitPay Inc.
// Copyright (c) 2014-2016 The Bitcoin Core developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or https://opensource.org/licenses/mit-license.php.

#include <stdint.h>
#include <vector>
#include <string>
#include <map>
#include <cassert>
#include <stdexcept>
#include <rlpvalue.h>

#define BOOST_FIXTURE_TEST_SUITE(a, b)
#define BOOST_AUTO_TEST_CASE(funcName) void funcName()
#define BOOST_AUTO_TEST_SUITE_END()
#define BOOST_CHECK(expr) assert(expr)
#define BOOST_CHECK_EQUAL(v1, v2) assert((v1) == (v2))
#define BOOST_CHECK_THROW(stmt, excMatch) { \
        try { \
            (stmt); \
            assert(0 && "No exception caught"); \
        } catch (excMatch & e) { \
	} catch (...) { \
	    assert(0 && "Wrong exception caught"); \
	} \
    }
#define BOOST_CHECK_NO_THROW(stmt) { \
        try { \
            (stmt); \
	} catch (...) { \
	    assert(0); \
	} \
    }

BOOST_FIXTURE_TEST_SUITE(rlpvalue_tests, BasicTestingSetup)

BOOST_AUTO_TEST_CASE(rlpvalue_constructor)
{
    RLPValue v2(RLPValue::VBUF);
    BOOST_CHECK(v2.isBuffer());

    RLPValue v3(RLPValue::VBUF);
    v3.assign("foo");
    BOOST_CHECK(v3.isBuffer());
    BOOST_CHECK_EQUAL(v3.getValStr(), "foo");

    std::string vs("yawn");
    RLPValue v8(vs);
    BOOST_CHECK(v8.isBuffer());
    BOOST_CHECK_EQUAL(v8.getValStr(), "yawn");

    const char *vcs = "zappa";
    RLPValue v9(vcs);
    BOOST_CHECK(v9.isBuffer());
    BOOST_CHECK_EQUAL(v9.getValStr(), "zappa");
}

BOOST_AUTO_TEST_CASE(rlpvalue_set)
{
    RLPValue v("foo");
    v.clear();
    BOOST_CHECK_EQUAL(v.getValStr(), "");

    BOOST_CHECK(v.setArray());
    BOOST_CHECK(v.isArray());
    BOOST_CHECK_EQUAL(v.size(), 0);

    v.assign("zum");
    BOOST_CHECK(v.isBuffer());
    BOOST_CHECK_EQUAL(v.getValStr(), "zum");
}

BOOST_AUTO_TEST_CASE(rlpvalue_array)
{
    RLPValue arr(RLPValue::VARR);

    std::string vStr("zippy");
    BOOST_CHECK(arr.push_back(vStr));

    const char *s = "pippy";
    BOOST_CHECK(arr.push_back(s));

    RLPValue v;

    std::vector<RLPValue> vec;
    v.assign("boing");
    vec.push_back(v);

    v.assign("going");
    vec.push_back(v);

    BOOST_CHECK(arr.push_backV(vec));

    BOOST_CHECK_EQUAL(arr.empty(), false);
    BOOST_CHECK_EQUAL(arr.size(), 4);

    BOOST_CHECK_EQUAL(arr[0].getValStr(), "zippy");
    BOOST_CHECK_EQUAL(arr[1].getValStr(), "pippy");
    BOOST_CHECK_EQUAL(arr[2].getValStr(), "boing");
    BOOST_CHECK_EQUAL(arr[3].getValStr(), "going");

    BOOST_CHECK_EQUAL(arr[999].getValStr(), "");

    arr.clear();
    BOOST_CHECK(arr.empty());
    BOOST_CHECK_EQUAL(arr.size(), 0);
}

BOOST_AUTO_TEST_CASE(rlpvalue_readwrite)
{
}

BOOST_AUTO_TEST_SUITE_END()

int main (int argc, char *argv[])
{
    rlpvalue_constructor();
    rlpvalue_set();
    rlpvalue_array();
    rlpvalue_readwrite();
    return 0;
}

