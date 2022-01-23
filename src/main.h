
#pragma once

#define APPNAME			"png2ps"
#define PROD_NAME		APPNAME
#define PROD_DESCR		"Pat's PNG to PS dither tool"
#define MAJOR_VER		0
#define MINOR_VER		1
#define PATCH_VER		0
#define CPYR_YEAR		"2022"
#define AUTHOR			"Patric Keller"
#define COMPANY			"BozzumDesign (c) (" AUTHOR ")"

typedef struct {
	int*	data;
	int		w;
	int		h;
} Img;

typedef struct {
	int		x;
	int		y;
} Pt;

typedef struct {
	int		num;
	int		den;
} Coeff;
