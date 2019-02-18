
/*
 * Copyright 2019 Bloq Inc.
 *
 */

#ifndef HAVE_CONFIG_H
#error missing autoconf-generated config.h.
#endif
#include "rlpvalue-config.h"

#include <vector>
#include <string>
#include <argp.h>
#include <univalue.h>
#include "rlpvalue.h"
#include "../test/utilstrencodings.h"

using namespace std;

extern void RLPtoJSON(const RLPValue& rval, UniValue& jval);
extern bool JSONtoRLP(const UniValue& jval, RLPValue& rval);

static const char doc[] =
"rlp - encode or decode RLP protocol encoding";

static struct argp_option options[] = {
	{ "decode", 'd', NULL, 0,
	  "Decode RLP to JSON" },
	{ "encode", 'e', NULL, 0,
	  "Encode JSON to RLP (default)" },
	{ }
};

static error_t parse_opt (int key, char *arg, struct argp_state *state);
static const struct argp argp = { options, parse_opt, NULL, doc };

static bool opt_decode = false;
static bool inp_json, inp_rlp, outp_json, outp_rlp;
static vector<string> file_args;
static UniValue global_jval;
static RLPValue global_rval;

static error_t parse_opt (int key, char *arg, struct argp_state *state)
{
	switch (key) {
	case 'd': opt_decode = true; break;
	case 'e': opt_decode = false; break;

	case ARGP_KEY_ARG:
		file_args.push_back(arg);
		break;

	default:
		return ARGP_ERR_UNKNOWN;
	}

	return 0;
}

static bool parseJsonInput(const std::string& body)
{
	bool rc = global_jval.read(body);

	if (!rc)
		fprintf(stderr, "JSON input validation failed\n");

	return rc;
}

static bool parseRlpInput(const std::string& body)
{
	std::vector<unsigned char> buf;

	if (body.substr(0, 2) == "0x") {
		string tmp = body.substr(2);
		buf = ParseHex(tmp);
	} else
		buf = ParseHex(body);

	size_t consumed, wanted;
	bool rc = global_rval.read(&buf[0], buf.size(), consumed, wanted);

	if (!rc)
		fprintf(stderr, "RLP input validation failed (%zu wanted)\n",
			wanted);

	return rc;
}

static bool readInput()
{
	if (opt_decode) {
		inp_json = false;
		inp_rlp = true;
		outp_json = true;
		outp_rlp = false;
	} else {
		inp_json = true;
		inp_rlp = false;
		outp_json = false;
		outp_rlp = true;
	}

	const char *filename = nullptr;
	FILE *inf;
	if (file_args.empty()) {
		filename = "(standard input)";
		inf = stdin;
	} else {
		filename = file_args[0].c_str();
		inf = ::fopen(filename, "r");
		if (!inf) {
			perror(filename);
			return false;
		}
	}

	string body;

	while (1) {
		vector<unsigned char> buf(4096);
		size_t rrc = fread(&buf[0], 1, buf.size(), inf);
		body.append((char *) &buf[0], rrc);
		if (rrc < buf.size())
			break;
	}
	bool haveErr = ferror(inf);
	if (inf != stdin)
		fclose(inf);

	if (haveErr) {
		perror(filename);
		return false;
	}

	if (inp_json)
		return parseJsonInput(body);
	else
		return parseRlpInput(body);
}

static bool mutateInput()
{
	if (inp_rlp)
		RLPtoJSON(global_rval, global_jval);
	else
		JSONtoRLP(global_jval, global_rval);
	return true;
}

static bool writeJsonOutput()
{
	string body = global_jval.write(2);
	printf("%s\n", body.c_str());
	return true;
}

static bool writeRlpOutput()
{
	string body = global_rval.write();
	string hex = HexStr(body.begin(), body.end());
	printf("%s\n", hex.c_str());
	return true;
}

static bool writeOutput()
{
	if (outp_json)
		return writeJsonOutput();
	else
		return writeRlpOutput();
}

int main (int argc, char *argv[])
{
	error_t argp_rc = argp_parse(&argp, argc, argv, 0, NULL, NULL);
	if (argp_rc) {
		fprintf(stderr, "%s: argp_parse failed: %s\n",
			argv[0], strerror(argp_rc));
		return EXIT_FAILURE;
	}

	if (!readInput() || !mutateInput() || !writeOutput())
		return 1;

	return 0;
}

