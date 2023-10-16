// This file is part of img2ps.
// Copyright (c) 2022-2023, Patric Keller
// The code is made available under the MIT License, see LICENSE document

#pragma once

#include "res/build.num"

#define APPNAME			"img2ps"
#define PROD_NAME		APPNAME
#define PROD_DESCR		"Pat's Image to PostScript dither tool"
#define MAJOR_VER		1
#define MINOR_VER		0
#define PATCH_VER		0
#define CPYR_YEAR		"2022-2023"
#define AUTHOR			"Patric Keller"
#define COMPANY			"BozzumDesign (" AUTHOR ")"
#define LICENSE			PROD_NAME " is made availabe under the MIT License"

typedef struct {
	int		num;
	int		den;
} Coeff;
