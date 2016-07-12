
#ifndef FTEST_INC
#define FTEST_INC

GAPIC_int ftest_get_level(GAPIC_float significance);
GAPIC_float critical_f(GAPIC_int level, GAPIC_int df);
GAPIC_int ftest(GAPIC_int level, GAPIC_float sst, GAPIC_float ssr, GAPIC_int df);

#endif
