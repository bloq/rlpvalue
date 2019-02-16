#include "rlpvalue.h"

int main (int argc, char *argv[])
{
    char buf[] = "___[1,2,3]___";
    RLPValue val;
    return val.read(buf + 3, 7) ? 0 : 1;
}
