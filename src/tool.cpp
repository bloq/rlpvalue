
/*
 * Copyright 2019 Bloq Inc.
 *
 */

#ifndef HAVE_CONFIG_H
#error missing autoconf-generated config.h.
#endif
#include "rlpvalue-config.h"

#include <argp.h>
#include <univalue.h>
#include "rlpvalue.h"

static const char doc[] =
"rlp - encode or decode RLP protocol encoding";

static struct argp_option options[] = {
	{ "decode", 'd', NULL, 0,
	  "Decode RLP to JSON" },
	{ "encode", 'e', NULL, 0,
	  "Encode JSON to RLP" },
	{ }
};

static error_t parse_opt (int key, char *arg, struct argp_state *state);
static const struct argp argp = { options, parse_opt, NULL, doc };

static bool opt_decode = true;

static error_t parse_opt (int key, char *arg, struct argp_state *state)
{
	switch (key) {
	case 'd': opt_decode = true; break;
	case 'e': opt_decode = false; break;

	default:
		return ARGP_ERR_UNKNOWN;
	}

	return 0;
}

int main (int argc, char *argv[])
{
	error_t argp_rc = argp_parse(&argp, argc, argv, 0, NULL, NULL);
	if (argp_rc) {
		fprintf(stderr, "%s: argp_parse failed: %s\n",
			argv[0], strerror(argp_rc));
		return EXIT_FAILURE;
	}

	return 0;
}

