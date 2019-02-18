
#include <vector>
#include <univalue.h>
#include "rlpvalue.h"

extern void RLPtoJSON(const RLPValue& rval, UniValue& jval);

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

