// Copyright 2014 BitPay Inc.
// Copyright 2015 Bitcoin Core Developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or https://opensource.org/licenses/mit-license.php.

#include <stdint.h>
#include <iomanip>
#include <sstream>
#include <stdlib.h>

#include "rlpvalue.h"

const RLPValue NullRLPValue;

void RLPValue::clear()
{
    typ = VSTR;
    val.clear();
    values.clear();
}

bool RLPValue::setStr(const std::string& val_)
{
    clear();
    typ = VSTR;
    val = val_;
    return true;
}

bool RLPValue::setArray()
{
    clear();
    typ = VARR;
    return true;
}

bool RLPValue::push_back(const RLPValue& val_)
{
    if (typ != VARR)
        return false;

    values.push_back(val_);
    return true;
}

bool RLPValue::push_backV(const std::vector<RLPValue>& vec)
{
    if (typ != VARR)
        return false;

    values.insert(values.end(), vec.begin(), vec.end());

    return true;
}

const RLPValue& RLPValue::operator[](size_t index) const
{
    if (typ != VARR)
        return NullRLPValue;
    if (index >= values.size())
        return NullRLPValue;

    return values.at(index);
}

const char *uvTypeName(RLPValue::VType t)
{
    switch (t) {
    case RLPValue::VARR: return "array";
    case RLPValue::VSTR: return "string";
    }

    // not reached
    return NULL;
}

