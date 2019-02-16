// Copyright 2014 BitPay Inc.
// Distributed under the MIT software license, see the accompanying
// file COPYING or https://opensource.org/licenses/mit-license.php.

#include <iomanip>
#include <stdio.h>
#include "rlpvalue.h"

std::string RLPValue::write() const
{
    std::string s;
    s.reserve(64);

    switch (typ) {
    case VNULL:
	assert(0 && "should not be writing this atom");
        break;
    case VARR:
        writeArray(s);
        break;
    case VSTR:
        writeString(s);
        break;
    case VNUM:
	assert(0 && "should not be writing this atom");
        break;
    }

    return s;
}

void RLPValue::writeString(std::string& s) const
{
	s += val;
}

void RLPValue::writeArray(std::string& s) const
{
    s += "[";

    for (unsigned int i = 0; i < values.size(); i++) {
        s += values[i].write();
    }

    s += "]";
}

