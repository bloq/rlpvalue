// Copyright (c) 2014 BitPay Inc.
// Copyright (c) 2014-2016 The Bitcoin Core developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or https://opensource.org/licenses/mit-license.php.

#include <stdint.h>
#include <string.h>
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
    unsigned char s1[] = { 0x83, 'd', 'o', 'g' };

    RLPValue v;
    size_t consumed, wanted;
    bool rc = v.read(&s1[0], sizeof(s1), consumed, wanted);
    BOOST_CHECK(rc);
    BOOST_CHECK(v.isBuffer());
    BOOST_CHECK_EQUAL(v.getValStr(), "dog");
    BOOST_CHECK_EQUAL(consumed, sizeof(s1));
    BOOST_CHECK_EQUAL(wanted, 0);

    std::string s1_out = v.write();
    BOOST_CHECK_EQUAL(sizeof(s1), s1_out.size());
    BOOST_CHECK_EQUAL(memcmp(&s1[0], &s1_out[0], sizeof(s1)), 0);

    unsigned char s2[] =
    { 0xc8, 0x83, 'c', 'a', 't', 0x83, 'd', 'o', 'g' };

    RLPValue v2;
    rc = v2.read(&s2[0], sizeof(s2), consumed, wanted);
    BOOST_CHECK(rc);
    BOOST_CHECK(v2.isArray());
    BOOST_CHECK_EQUAL(consumed, sizeof(s2));
    BOOST_CHECK_EQUAL(wanted, 0);
    BOOST_CHECK_EQUAL(v2[0].getValStr(), "cat");
    BOOST_CHECK_EQUAL(v2[1].getValStr(), "dog");

    std::string s2_out = v2.write();
    BOOST_CHECK_EQUAL(sizeof(s2), s2_out.size());
    BOOST_CHECK_EQUAL(memcmp(&s2[0], &s2_out[0], sizeof(s2)), 0);

    unsigned char s_empty_list[] = { 0xc0 };
    RLPValue v_empty_list;
    rc = v_empty_list.read(&s_empty_list[0], sizeof(s_empty_list), consumed, wanted);
    BOOST_CHECK(rc);
    BOOST_CHECK(v_empty_list.isArray());
    BOOST_CHECK_EQUAL(v_empty_list.size(), 0);

    unsigned char s_null[] = { 0x80 };
    RLPValue v_null;
    rc = v_null.read(&s_null[0], sizeof(s_null), consumed, wanted);
    BOOST_CHECK(rc);
    BOOST_CHECK(v_null.isBuffer());
    BOOST_CHECK_EQUAL(v_null.getValStr(), "");
    BOOST_CHECK_EQUAL(consumed, sizeof(s_null));
    BOOST_CHECK_EQUAL(wanted, 0);
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

