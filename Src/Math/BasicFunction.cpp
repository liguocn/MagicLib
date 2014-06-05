#include "BasicFunction.h"
#include <math.h>

namespace MagicMath
{
    BasicFunction::BasicFunction()
    {
    }

    BasicFunction::~BasicFunction()
    {
    }

    double BasicFunction::Sigmoid(double x)
    {
        return 1.0 / (1.0 + exp(-x));
    }
}
