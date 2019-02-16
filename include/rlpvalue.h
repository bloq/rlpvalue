// Copyright 2014 BitPay Inc.
// Copyright 2015 Bitcoin Core Developers
// Distributed under the MIT software license, see the accompanying
// file COPYING or https://opensource.org/licenses/mit-license.php.

#ifndef __RLPVALUE_H__
#define __RLPVALUE_H__

#include <stdint.h>
#include <string.h>

#include <string>
#include <vector>
#include <map>
#include <cassert>

#include <utility>        // std::pair

class RLPValue {
public:
    enum VType { VARR, VSTR, };

    RLPValue() { typ = VSTR; }
    RLPValue(RLPValue::VType initialType, const std::string& initialStr = "") {
        typ = initialType;
        val = initialStr;
    }
    RLPValue(const std::string& val_) {
        setStr(val_);
    }
    RLPValue(const char *val_) {
        std::string s(val_);
        setStr(s);
    }
    ~RLPValue() {}

    void clear();

    bool setStr(const std::string& val);
    bool setArray();

    enum VType getType() const { return typ; }
    const std::string& getValStr() const { return val; }
    bool empty() const { return (values.size() == 0); }

    size_t size() const { return values.size(); }

    const RLPValue& operator[](size_t index) const;

    bool isStr() const { return (typ == VSTR); }
    bool isArray() const { return (typ == VARR); }

    bool push_back(const RLPValue& val);
    bool push_back(const std::string& val_) {
        RLPValue tmpVal(VSTR, val_);
        return push_back(tmpVal);
    }
    bool push_back(const char *val_) {
        std::string s(val_);
        return push_back(s);
    }
    bool push_backV(const std::vector<RLPValue>& vec);

    std::string write() const;

    bool read(const char *raw, size_t len);
    bool read(const char *raw) { return read(raw, strlen(raw)); }
    bool read(const std::string& rawStr) {
        return read(rawStr.data(), rawStr.size());
    }

private:
    RLPValue::VType typ;
    std::string val;                       // numbers are stored as C++ strings
    std::vector<RLPValue> values;

    void writeString(std::string& s) const;
    void writeArray(std::string& s) const;

public:
    // Strict type-specific getters, these throw std::runtime_error if the
    // value is of unexpected type
    const std::vector<RLPValue>& getValues() const;
    const std::string& get_str() const;
    const RLPValue& get_array() const;

    enum VType type() const { return getType(); }
};

enum jtokentype {
    JTOK_ERR        = -1,
    JTOK_NONE       = 0,                           // eof
    JTOK_OBJ_OPEN,
    JTOK_OBJ_CLOSE,
    JTOK_ARR_OPEN,
    JTOK_ARR_CLOSE,
    JTOK_COLON,
    JTOK_COMMA,
    JTOK_KW_NULL,
    JTOK_KW_TRUE,
    JTOK_KW_FALSE,
    JTOK_NUMBER,
    JTOK_STRING,
};

extern enum jtokentype getJsonToken(std::string& tokenVal,
                                    unsigned int& consumed, const char *raw, const char *end);
extern const char *uvTypeName(RLPValue::VType t);

static inline bool jsonTokenIsValue(enum jtokentype jtt)
{
    switch (jtt) {
    case JTOK_KW_NULL:
    case JTOK_KW_TRUE:
    case JTOK_KW_FALSE:
    case JTOK_NUMBER:
    case JTOK_STRING:
        return true;

    default:
        return false;
    }

    // not reached
}

static inline bool json_isspace(int ch)
{
    switch (ch) {
    case 0x20:
    case 0x09:
    case 0x0a:
    case 0x0d:
        return true;

    default:
        return false;
    }

    // not reached
}

extern const RLPValue NullRLPValue;

#endif // __RLPVALUE_H__
