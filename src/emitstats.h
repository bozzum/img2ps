// This file is part of img2ps.
// Copyright (c) 2022-2023, Patric Keller (bozzumdesign at gmail dot com)
// The code is made available under the MIT License, see LICENSE document

#pragma once

#include <cstdio>
#include "main.h"
#include "analyse.h"

// Function: emitstats
// Renders the statistic data of the image to a GRAP file.
//
// Parameters:
//   out - Handle of the GRAP output file
//   stat - Statistic data
//
// Returns:
//   Returns 0.
//
int emitstats(FILE* out, const Stat& stat);
