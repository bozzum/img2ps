
#pragma once

#include <array>
#include <climits>
#include "main.h"

typedef struct {
	std::array<int, 256>	lCnt	= {};
	int						sumLuma	= {0};
	int						minLuma	= {255};
	int						maxLuma	= {0};
	int						avrLuma	= {0};
	int						sumCnt	= {0};
	int						unique	= {0};
	int						minCnt	= {INT_MAX};
	int						maxCnt	= {0};
} Stat;

int analyse(const Img& img, Stat& stat);
