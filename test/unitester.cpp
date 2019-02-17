// Copyright 2014 BitPay Inc.
// Distributed under the MIT/X11 software license, see the accompanying
// file COPYING or https://opensource.org/licenses/mit-license.php.

#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <cassert>
#include <string>
#include <univalue.h>
#include "utilstrencodings.h"
#include "rlpvalue.h"

using namespace std;

#ifndef JSON_TEST_SRC
#error JSON_TEST_SRC must point to test source directory
#endif

#ifndef ARRAY_SIZE
#define ARRAY_SIZE(arr) (sizeof(arr) / sizeof((arr)[0]))
#endif

std::string srcdir(JSON_TEST_SRC);
static bool test_failed = false;

static bool runtest(const std::string& filename, const std::string& key,
		    const UniValue& jval)
{
	if (!jval.isObject() ||
	    !jval["in"].isStr() ||
	    !jval["out"].isStr()) {
		fprintf(stderr, "    %s: skipping test, invalid\n",
			key.c_str());
		return true;
	}

	const string& ins = jval["in"].getValStr();
	const string& outs = jval["out"].getValStr();

	std::vector<unsigned char> outb = ParseHex(outs);

	RLPValue v;
	size_t consumed, wanted;
	bool rrc = v.read(&outb[0], outb.size(), consumed, wanted);

	bool rc = false;
	if (ins == "VALID" || ins == "INVALID") {
		if (ins == "VALID" && rrc == true)
			rc = true;
		else if (ins == "INVALID" && rrc == false)
			rc = true;
	} else {
		assert(0 && "unknown in-string in JSON");
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
//	"rlptest.json",
};

int main (int argc, char *argv[])
{
    for (unsigned int fidx = 0; fidx < ARRAY_SIZE(filenames); fidx++) {
        runtest_file(filenames[fidx]);
    }

    return test_failed ? 1 : 0;
}

