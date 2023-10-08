#include "Gene.h"

Gene::Gene(int in, int out, int innovation, bool enabled, double weight) : in(in), out(out),
                                                                           innovation(innovation),
                                                                           enabled(enabled),
                                                                           weight(weight) {}

Gene &Gene::operator=(const Gene &gene) = default;

Gene::Gene() : Gene(0, 0, 0, false, 0) {}
#define DUPA xd
Gene::Gene(const Gene &gene) = default;
