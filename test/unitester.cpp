// Copyright 2014 BitPay Inc.
// Distributed under the MIT/X11 software license, see the accompanying
// file COPYING or https://opensource.org/licenses/mit-license.php.

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <cassert>
#include <string>
#include <ctype.h>
#include <univalue.h>
#include "utilstrencodings.h"
#include "rlpvalue.h"
#include "../src/InfInt.h"

using namespace std;

#ifndef JSON_TEST_SRC
#error JSON_TEST_SRC must point to test source directory
#endif

#ifndef ARRAY_SIZE
#define ARRAY_SIZE(arr) (sizeof(arr) / sizeof((arr)[0]))
#endif

std::string srcdir(JSON_TEST_SRC);
static bool test_failed = false;

extern uint64_t toInteger(const unsigned char *raw, size_t len);
extern std::string encodeBinary(uint64_t n);
extern bool JSONtoRLP(const UniValue& jval, RLPValue& rval);

static uint64_t toInteger(const char *raw, size_t len) {
	return toInteger((const unsigned char *) raw, len);
}

static bool isBigNumStr(const std::string& s)
{
	// first char must be #
	if (s.empty() || s[0] != '#')
		return false;

	// remaining chars must be digits
	for (unsigned int i = 1; i < s.size(); i++)
		if (!isdigit(s[i]))
			return false;

	return true;
}

static std::string encodeBigNum(const InfInt& n)
{
	std::string rs;

	if (n == 0) {
		// do nothing; return empty string
	} else {
		rs.assign(encodeBigNum(n / 256));

		InfInt iich = n % 256;
		unsigned char ch = iich.toUnsignedLong();
		rs.append((const char *) &ch, 1);
	}

	return rs;
}

static std::string encodeBigNumStr(const std::string& s)
{
	InfInt n(s);
	return encodeBigNum(n);
}

static bool runtest(const std::string& filename, const std::string& key,
		    const UniValue& jval)
{
	if (!jval.isObject() ||
	    !jval["out"].isStr()) {
		fprintf(stderr, "    %s: skipping test, invalid\n",
			key.c_str());
		return true;
	}

	string ins = jval["in"].getValStr();
	string outs = jval["out"].getValStr();

	if (outs.substr(0, 2) == "0x")		// remove 0x prefix
		outs = outs.substr(2);

	// decode RLP binary output test string from hex to binary
	std::vector<unsigned char> outb = ParseHex(outs);

	// attempt to parse with RLP class
	RLPValue v;
	size_t consumed, wanted;
	bool rrc = v.read(&outb[0], outb.size(), consumed, wanted);

	bool rc = false;
	if (ins == "VALID" || ins == "INVALID") {
		if (ins == "VALID" && rrc == true)
			rc = true;
		else if (ins == "INVALID" && rrc == false)
			rc = true;
	} else if (jval["in"].isStr()) {
		if (isBigNumStr(ins))
			ins = encodeBigNumStr(ins.substr(1));

		if (v.isBuffer() && (ins == v.getValStr()))
			rc = true;

	} else if (jval["in"].isNum()) {
		if (v.isBuffer()) {
			uint64_t test_val = jval["in"].get_int64();
			string enc_val = v.getValStr();
			uint64_t dec_val = toInteger(&enc_val[0],
						     enc_val.size());
			if (test_val == dec_val)
				rc = true;
		}
	} else if (jval["in"].isArray()) {
		RLPValue root(RLPValue::VARR);
		JSONtoRLP(jval["in"], root);

		std::string genOutput = root.write();
		if ((outb.size() == genOutput.size()) &&
		    (memcmp(&outb[0], &genOutput[0], outb.size()) == 0)) {
			rc = true;
		} else {
			std::string genHex = HexStr(genOutput.begin(),
						    genOutput.end());
			fprintf(stderr, "INS :%s\nGENS:%s\n",
				outs.c_str(),
				genHex.c_str());
		}

	} else {
		fprintf(stderr, "ERR: test %s not implemented yet\n", key.c_str());
	}

	fprintf(stderr, "    %s: %s\n", key.c_str(),
		rc ? "ok" : "FAIL");
	return rc;
}

static void runtest_jfile(std::string filename, const UniValue& jfile)
{
	assert(jfile.isObject());

	const std::vector<std::string>& keys = jfile.getKeys();

	fprintf(stderr, "Running testfile %s (%zu tests)\n",
		filename.c_str(), keys.size());

	for (auto it = keys.begin(); it != keys.end(); it++) {
		const std::string& key = *it;
		if (!runtest(filename, key, jfile[key]))
			test_failed = true;
	}
}

static void runtest_file(const char *filename_)
{
        std::string basename(filename_);
        std::string filename = srcdir + "/" + basename;
        FILE *f = fopen(filename.c_str(), "r");
	if (!f) {
		perror(filename.c_str());
		exit(1);
	}

        std::string jdata;

        char buf[4096];
        while (!feof(f)) {
                int bread = fread(buf, 1, sizeof(buf), f);
                assert(!ferror(f));

                std::string s(buf, bread);
                jdata += s;
        }

        assert(!ferror(f));
        fclose(f);

	UniValue jfile;
	bool rc = jfile.read(jdata);
	assert(rc == true);

        runtest_jfile(basename, jfile);
}

static const char *filenames[] = {
	"example.json",
	"invalidRLPTest.json",
	"rlptest.json",
//	"longlist.json",
};

int main (int argc, char *argv[])
{
    for (unsigned int fidx = 0; fidx < ARRAY_SIZE(filenames); fidx++) {
        runtest_file(filenames[fidx]);
    }

    return test_failed ? 1 : 0;
}

