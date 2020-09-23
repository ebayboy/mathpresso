
#include <cstring>
#include <iostream>

#include <mathpresso/mathpresso.h>

using namespace std;

mathpresso::Context ctx;
mathpresso::Expression exp;

void init()
{
    ctx.addBuiltIns();

    ctx.addVariable("x", 0 * sizeof(double));
    ctx.addVariable("y", 1 * sizeof(double));

    mathpresso::Error err = exp.compile(ctx, "(5.5 + x) + (2 * x - 2 / 3 * y) * (x / 3 + y / 4) + (y + 7.7)", mathpresso::kNoOptions);
    if (err != mathpresso::kErrorOk) {
        printf("Expression Error: %u\n", err);
		exit(1);
    }
}

void compute()
{
	static double data[] = { 1.2, 3.8 };
	exp.evaluate(data);
}

