// This file is part of img2ps.
// Copyright (c) 2022-2023, Patric Keller (bozzumdesign at gmail dot com)
// The code is made available under the MIT License, see LICENSE document

#pragma once

#include <array>
#include <climits>
#include "main.h"
#include "img.h"

typedef struct {
	std::array<int, 256>	lCnt	= {};			// array of luma counts
	int						sumLuma	= {0};			// total luma count
	int						minLuma	= {255};		// lowest luma value
	int						maxLuma	= {0};			// highest luma value
	int						avrLuma	= {0};			// average luma value
	int						sumCnt	= {0};			// number of values examined
	int						unique	= {0};			// number of unique luma vals
	int						minCnt	= {INT_MAX};	// lowest number of unique vals
	int						maxCnt	= {0};			// highest number of unique vals
} Stat;

// Function: analyse
// Analyses basic image properties, like minimum, maximum and average lume values,
// plus some data about their distribution. This data will also be used to generate
// the histogram with <emitstats>.
//
// Parameters:
//   img - Source image buffer
//   stat - Statistics structure, will be filled in by analyse()
//
// Returns:
//   Returns 0 on success, throws an exception on failure
//
int analyse(const Img<int>& img, Stat& stat);
