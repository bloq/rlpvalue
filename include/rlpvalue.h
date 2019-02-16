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
    enum VType { VNULL, VOBJ, VARR, VSTR, VNUM, VBOOL, };

    RLPValue() { typ = VNULL; }
    RLPValue(RLPValue::VType initialType, const std::string& initialStr = "") {
        typ = initialType;
        val = initialStr;
    }
    RLPValue(uint64_t val_) {
        setInt(val_);
    }
    RLPValue(int64_t val_) {
        setInt(val_);
    }
    RLPValue(bool val_) {
        setBool(val_);
    }
    RLPValue(int val_) {
        setInt(val_);
    }
    RLPValue(double val_) {
        setFloat(val_);
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

    bool setNull();
    bool setBool(bool val);
    bool setNumStr(const std::string& val);
    bool setInt(uint64_t val);
    bool setInt(int64_t val);
    bool setInt(int val_) { return setInt((int64_t)val_); }
    bool setFloat(double val);
    bool setStr(const std::string& val);
    bool setArray();
    bool setObject();

    enum VType getType() const { return typ; }
    const std::string& getValStr() const { return val; }
    bool empty() const { return (values.size() == 0); }

    size_t size() const { return values.size(); }

    bool getBool() const { return isTrue(); }
    void getObjMap(std::map<std::string,RLPValue>& kv) const;
    bool checkObject(const std::map<std::string,RLPValue::VType>& memberTypes) const;
    const RLPValue& operator[](const std::string& key) const;
    const RLPValue& operator[](size_t index) const;
    bool exists(const std::string& key) const { size_t i; return findKey(key, i); }

    bool isNull() const { return (typ == VNULL); }
    bool isTrue() const { return (typ == VBOOL) && (val == "1"); }
    bool isFalse() const { return (typ == VBOOL) && (val != "1"); }
    bool isBool() const { return (typ == VBOOL); }
    bool isStr() const { return (typ == VSTR); }
    bool isNum() const { return (typ == VNUM); }
    bool isArray() const { return (typ == VARR); }
    bool isObject() const { return (typ == VOBJ); }

    bool push_back(const RLPValue& val);
    bool push_back(const std::string& val_) {
        RLPValue tmpVal(VSTR, val_);
        return push_back(tmpVal);
    }
    bool push_back(const char *val_) {
        std::string s(val_);
        return push_back(s);
    }
    bool push_back(uint64_t val_) {
        RLPValue tmpVal(val_);
        return push_back(tmpVal);
    }
    bool push_back(int64_t val_) {
        RLPValue tmpVal(val_);
        return push_back(tmpVal);
    }
    bool push_back(int val_) {
        RLPValue tmpVal(val_);
        return push_back(tmpVal);
    }
    bool push_back(double val_) {
        RLPValue tmpVal(val_);
        return push_back(tmpVal);
    }
    bool push_backV(const std::vector<RLPValue>& vec);

    void __pushKV(const std::string& key, const RLPValue& val);
    bool pushKV(const std::string& key, const RLPValue& val);
    bool pushKV(const std::string& key, const std::string& val_) {
        RLPValue tmpVal(VSTR, val_);
        return pushKV(key, tmpVal);
    }
    bool pushKV(const std::string& key, const char *val_) {
        std::string _val(val_);
        return pushKV(key, _val);
    }
    bool pushKV(const std::string& key, int64_t val_) {
        RLPValue tmpVal(val_);
        return pushKV(key, tmpVal);
    }
    bool pushKV(const std::string& key, uint64_t val_) {
        RLPValue tmpVal(val_);
        return pushKV(key, tmpVal);
    }
    bool pushKV(const std::string& key, bool val_) {
        RLPValue tmpVal((bool)val_);
        return pushKV(key, tmpVal);
    }
    bool pushKV(const std::string& key, int val_) {
        RLPValue tmpVal((int64_t)val_);
        return pushKV(key, tmpVal);
    }
    bool pushKV(const std::string& key, double val_) {
        RLPValue tmpVal(val_);
        return pushKV(key, tmpVal);
    }
    bool pushKVs(const RLPValue& obj);

    std::string write(unsigned int prettyIndent = 0,
                      unsigned int indentLevel = 0) const;

    bool read(const char *raw, size_t len);
    bool read(const char *raw) { return read(raw, strlen(raw)); }
    bool read(const std::string& rawStr) {
        return read(rawStr.data(), rawStr.size());
    }

private:
    RLPValue::VType typ;
    std::string val;                       // numbers are stored as C++ strings
    std::vector<std::string> keys;
    std::vector<RLPValue> values;

    bool findKey(const std::string& key, size_t& retIdx) const;
    void writeArray(unsigned int prettyIndent, unsigned int indentLevel, std::string& s) const;
    void writeObject(unsigned int prettyIndent, unsigned int indentLevel, std::string& s) const;

public:
    // Strict type-specific getters, these throw std::runtime_error if the
    // value is of unexpected type
    const std::vector<std::string>& getKeys() const;
    const std::vector<RLPValue>& getValues() const;
    bool get_bool() const;
    const std::string& get_str() const;
    int get_int() const;
    int64_t get_int64() const;
    double get_real() const;
    const RLPValue& get_obj() const;
    const RLPValue& get_array() const;

    enum VType type() const { return getType(); }
    bool push_back(std::pair<std::string,RLPValue> pear) {
        return pushKV(pear.first, pear.second);
    }
    friend const RLPValue& find_value( const RLPValue& obj, const std::string& name);
};

//
// The following were added for compatibility with json_spirit.
// Most duplicate other methods, and should be removed.
//
static inline std::pair<std::string,RLPValue> Pair(const char *cKey, const char *cVal)
{
    std::string key(cKey);
    RLPValue uVal(cVal);
    return std::make_pair(key, uVal);
}

static inline std::pair<std::string,RLPValue> Pair(const char *cKey, std::string strVal)
{
    std::string key(cKey);
    RLPValue uVal(strVal);
    return std::make_pair(key, uVal);
}

static inline std::pair<std::string,RLPValue> Pair(const char *cKey, uint64_t u64Val)
{
    std::string key(cKey);
    RLPValue uVal(u64Val);
    return std::make_pair(key, uVal);
}

static inline std::pair<std::string,RLPValue> Pair(const char *cKey, int64_t i64Val)
{
    std::string key(cKey);
    RLPValue uVal(i64Val);
    return std::make_pair(key, uVal);
}

static inline std::pair<std::string,RLPValue> Pair(const char *cKey, bool iVal)
{
    std::string key(cKey);
    RLPValue uVal(iVal);
    return std::make_pair(key, uVal);
}

static inline std::pair<std::string,RLPValue> Pair(const char *cKey, int iVal)
{
    std::string key(cKey);
    RLPValue uVal(iVal);
    return std::make_pair(key, uVal);
}

static inline std::pair<std::string,RLPValue> Pair(const char *cKey, double dVal)
{
    std::string key(cKey);
    RLPValue uVal(dVal);
    return std::make_pair(key, uVal);
}

static inline std::pair<std::string,RLPValue> Pair(const char *cKey, const RLPValue& uVal)
{
    std::string key(cKey);
    return std::make_pair(key, uVal);
}

static inline std::pair<std::string,RLPValue> Pair(std::string key, const RLPValue& uVal)
{
    return std::make_pair(key, uVal);
}

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

const RLPValue& find_value( const RLPValue& obj, const std::string& name);

#endif // __RLPVALUE_H__
