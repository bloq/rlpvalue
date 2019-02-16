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
    typ = VNULL;
    val.clear();
    values.clear();
}

bool RLPValue::setNull()
{
    clear();
    return true;
}

static bool validNumStr(const std::string& s)
{
    std::string tokenVal;
    unsigned int consumed;
    enum jtokentype tt = getJsonToken(tokenVal, consumed, s.data(), s.data() + s.size());
    return (tt == JTOK_NUMBER);
}

bool RLPValue::setNumStr(const std::string& val_)
{
    if (!validNumStr(val_))
        return false;

    clear();
    typ = VNUM;
    val = val_;
    return true;
}

bool RLPValue::setInt(uint64_t val_)
{
    std::ostringstream oss;

    oss << val_;

    return setNumStr(oss.str());
}

bool RLPValue::setInt(int64_t val_)
{
    std::ostringstream oss;

    oss << val_;

    return setNumStr(oss.str());
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
    case RLPValue::VNULL: return "null";
    case RLPValue::VARR: return "array";
    case RLPValue::VSTR: return "string";
    case RLPValue::VNUM: return "number";
    }

    // not reached
    return NULL;
}

