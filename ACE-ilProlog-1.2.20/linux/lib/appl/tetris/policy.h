
#ifndef TETRIS_POLICY
#define TETRIS_POLICY

#include <cstdlib>
#include <iostream>
using namespace std;
#include "../../gapic/gapicgc.h"
#include "../../gapic/refcount.h"

void policy(TetrisState* stptr, int &anscol, int &ansori);

#endif
