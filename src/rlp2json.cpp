
#include <vector>
#include <univalue.h>
#include "rlpvalue.h"
#include "InfInt.h"

extern void RLPtoJSON(const RLPValue& rval, UniValue& jval);
extern std::string encodeBinary(uint64_t n);

static void assignJsonArray(UniValue& jval, const RLPValue& rval)
{
	jval.setArray();

	const std::vector<RLPValue>& values = rval.getValues();
	for (auto it = values.begin(); it != values.end(); it++) {
		const RLPValue& childVal = *it;
		UniValue childJval;

		RLPtoJSON(childVal, childJval);

		jval.push_back(childJval);
	}
}

static void assignJsonBuffer(UniValue& jval, const RLPValue& rval)
{
	jval.setStr(rval.getValStr());
}

void RLPtoJSON(const RLPValue& rval, UniValue& jval)
{
	if (rval.isBuffer())
		assignJsonBuffer(jval, rval);
	else
		assignJsonArray(jval, rval);
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

bool JSONtoRLP(const UniValue& jval, RLPValue& rval)
{
	if (jval.isStr()) {
		std::string ins = jval.getValStr();
		if (isBigNumStr(ins))
			ins = encodeBigNumStr(ins.substr(1));

		rval.assign(ins);
		return true;
	}

	if (jval.isNum()) {
		uint64_t val = jval.get_int64();
		std::string val_enc = encodeBinary(val);
		rval.assign(val_enc);
		return true;
	}

	if (jval.isArray()) {
		rval.setArray();
		const std::vector<UniValue>& arrVals = jval.getValues();
		for (auto it = arrVals.begin(); it != arrVals.end(); it++) {
			const UniValue& childJval = *it;
			RLPValue tmp;
			if (!JSONtoRLP(childJval, tmp))
				return false;
			rval.push_back(tmp);
		}
		return true;
	}

	return false;
}
