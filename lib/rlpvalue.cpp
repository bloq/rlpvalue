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
    keys.clear();
    values.clear();
}

bool RLPValue::setNull()
{
    clear();
    return true;
}

bool RLPValue::setBool(bool val_)
{
    clear();
    typ = VBOOL;
    if (val_)
        val = "1";
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

bool RLPValue::setFloat(double val_)
{
    std::ostringstream oss;

    oss << std::setprecision(16) << val_;

    bool ret = setNumStr(oss.str());
    typ = VNUM;
    return ret;
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

bool RLPValue::setObject()
{
    clear();
    typ = VOBJ;
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

void RLPValue::__pushKV(const std::string& key, const RLPValue& val_)
{
    keys.push_back(key);
    values.push_back(val_);
}

bool RLPValue::pushKV(const std::string& key, const RLPValue& val_)
{
    if (typ != VOBJ)
        return false;

    size_t idx;
    if (findKey(key, idx))
        values[idx] = val_;
    else
        __pushKV(key, val_);
    return true;
}

bool RLPValue::pushKVs(const RLPValue& obj)
{
    if (typ != VOBJ || obj.typ != VOBJ)
        return false;

    for (size_t i = 0; i < obj.keys.size(); i++)
        __pushKV(obj.keys[i], obj.values.at(i));

    return true;
}

void RLPValue::getObjMap(std::map<std::string,RLPValue>& kv) const
{
    if (typ != VOBJ)
        return;

    kv.clear();
    for (size_t i = 0; i < keys.size(); i++)
        kv[keys[i]] = values[i];
}

bool RLPValue::findKey(const std::string& key, size_t& retIdx) const
{
    for (size_t i = 0; i < keys.size(); i++) {
        if (keys[i] == key) {
            retIdx = i;
            return true;
        }
    }

    return false;
}

bool RLPValue::checkObject(const std::map<std::string,RLPValue::VType>& t) const
{
    if (typ != VOBJ)
        return false;

    for (std::map<std::string,RLPValue::VType>::const_iterator it = t.begin();
         it != t.end(); ++it) {
        size_t idx = 0;
        if (!findKey(it->first, idx))
            return false;

        if (values.at(idx).getType() != it->second)
            return false;
    }

    return true;
}

const RLPValue& RLPValue::operator[](const std::string& key) const
{
    if (typ != VOBJ)
        return NullRLPValue;

    size_t index = 0;
    if (!findKey(key, index))
        return NullRLPValue;

    return values.at(index);
}

const RLPValue& RLPValue::operator[](size_t index) const
{
    if (typ != VOBJ && typ != VARR)
        return NullRLPValue;
    if (index >= values.size())
        return NullRLPValue;

    return values.at(index);
}

const char *uvTypeName(RLPValue::VType t)
{
    switch (t) {
    case RLPValue::VNULL: return "null";
    case RLPValue::VBOOL: return "bool";
    case RLPValue::VOBJ: return "object";
    case RLPValue::VARR: return "array";
    case RLPValue::VSTR: return "string";
    case RLPValue::VNUM: return "number";
    }

    // not reached
    return NULL;
}

const RLPValue& find_value(const RLPValue& obj, const std::string& name)
{
    for (unsigned int i = 0; i < obj.keys.size(); i++)
        if (obj.keys[i] == name)
            return obj.values.at(i);

    return NullRLPValue;
}

