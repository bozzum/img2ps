// This file is part of img2ps.
// Copyright (c) 2022-2023, Patric Keller (bozzumdesign at gmail dot com)
// The code is made available under the MIT License, see LICENSE document

#include <getopt.h>
#include "loadfile.h"
#include "analyse.h"
#include "lumaproc.h"
#include "emitstats.h"
#include "dither.h"
#include "border.h"
#include "emitps.h"
#include "emitpng.h"
#include "img.h"
#include "main.h"

static void
printUsage()
{
	fprintf(stderr,
		"Usage: " APPNAME " [options] <img-file>\n"
		"\nImg-File:"
		"\n *.png\t\tPNG image"
		"\n *.jpg/jpeg\tJPEG image"
		"\n ::test\t\tInternal test pattern"
		"\nOptions:"
		"\n -b num\t\tBorder width [default: 0]"
		"\n -c algo\tChroma algorithm flags: C, B, S [default: C], PNG input only"
		"\n -d algo\tDither algorithm: TH, FS, JJN, SI [default: JJN]"
		"\n -l algo\tLuma algorithm flags: N, S, T [default: ST]"
		"\n -o file\tOutput file name [default: stdout]"
		"\n -p num\t\tPixel Per Inch [default: 96], used for test-pattern"
		"\n -s file\tStats file name [default: null]"
		"\n -t num\t\tOverwrite luma threshold level: 0..255 [default: -1], mainly for '-d TH'"
		"\n -V\t\tDisplay version info"
		"\n -?\t\tDisplay this help message"
	);

	fprintf(stderr, "\n\n"
		"Dither Algorithms:\n"
		"TH -- Threshold. This is the most simplest way of dithering, as it only\n"
		"has a single threshold for deciding between black and white. This algo-\n"
		"rithm has limited use, mainly for dithering text or drawings. Use the\n"
		"-t <val> option to optimize the threshold level.\n"
		"FS -- Floyd-Steinberg. It was first published in 1976 by Robert W. \n"
		"Floyd and Louis Steinberg. It is an Error-diffusion dithering that \n"
		"diffuses the quantization error to neighboring pixels.\n"
		"JJN -- Jarvis, Judice & Ninke. This algorithm tends to produce coarser\n"
		"patterns than the Floyd-Steinberg Dithering method, but with fewer\n"
		"artifacts.\n"
		"SI -- Sierra. Similar to Jarvis algorithm, probably coarsest of the\n"
		"three algorithms.\n"
		"Note: you can get a good first impression of the differences between\n"
		"the dither algorithms by using the test pattern as a source.\n"
		"\n"
		"Luma Algorithm Flags:\n"
		"N -- No Luma Threshold or Image Optimisation. This flag cannot be com-\n"
		"bined with any other luma flags.\n"
		"S -- Stretches the luma range of the image. This is useful for images\n"
		"with low contrast. This algorithm does not change the B/W threshold.\n"
		"T -- Change the B/W threshold to the average luma value. This can be\n"
		"particularly effective if combined with the 'S' (stretch) algorithm.\n"
		"\n"
		"Chroma to Luma Conversion Algorithm (PNG input only):\n"
		"C -- CCIR 601 (R: 0.299, G: 0.587, B: 0.114).\n"
		"B -- BT709 (R: 0.2126, G: 0.7152, B: 0.0722).\n"
		"S -- SMPTE 240M (R: 0.212, G: 0.701, B: 0.087).\n"
		"\n"
		"If a Stats file name has been specified, img2ps will output an additional\n"
		"GRAP file (Kernighan and Bentley's language for typesetting graphs),\n"
		"which can be used to produce a histogram display of the source image file.\n"
		"\n"
		"The internal test pattern generator (\"::test\" as input file name) can\n"
		"be used to verify and calibrate the printer output. The shape should be\n"
		"a perfect circle within a perfect square.\n"
		"\n"
		"If the output file extension is '.png' the output will be stored as a\n"
		"file! Bonus feature...!\n"
		"\n"
	);
}

static void
printVersion()
{
	printf("\n" APPNAME " - " PROD_DESCR "\n");
	printf("Version: %i.%i.%i - BuildNo: %i\n", MAJOR_VER, MINOR_VER, PATCH_VER, BUILD_NUM);
	printf("Copyright (c) " CPYR_YEAR ", " COMPANY "\n");
	printf(LICENSE "\n\n");
}

static void
printError(const std::string& str)
{
	fprintf(stderr, "\n" APPNAME ": Error: %s\n", str.c_str());
}

int
main(int argc, char** argv)
{
	FILE* out = stdout;
	FILE* ana = nullptr;
	int*  buf = nullptr;
	int rc = 1;

	try {
		std::string outName("");
		std::string lumaAlgo("ST");
		std::string chromaAlgo("C");
		std::string ditherAlgo("JJN");
		int ppi = 96;
		int th = 128;
		int border = 0;
		int overTh = -1;

		// get command-line options
		for(int o; (o = getopt(argc, argv, "?b:c:d:l:o:p:s:t:V")) != -1; )
			switch(o) {
				case '?': printUsage(); return 0;
				case 'b': border = (int)strtoul(optarg, nullptr, 0); break;
				case 'c': chromaAlgo = optarg; break;
				case 'd': ditherAlgo = optarg; break;
				case 'l': lumaAlgo = optarg; break;
				case 'o': out = fopen(optarg, "wb"); outName = {optarg}; break;
				case 'p': ppi = (int)strtoul(optarg, nullptr, 0); break;
				case 's': ana = fopen(optarg, "wb"); break;
				case 't': overTh = (int)strtoul(optarg, nullptr, 0); break;
				case 'V': printVersion(); return 0;
			}

		// we *need* an input image file...
		if(optind == argc) {
			printError("Missing Source Image File");
			printUsage();
			return 1;
		}

		if(not out)
			throw std::string("Cannot open output file");

		auto fname = argv[optind];

		// .........................................................................
		// load the input file or create a test pattern

		int width = 0, height = 0;
		buf = loadFile(fname, chromaAlgo, width, height, ppi);
		if(not buf)
			throw std::string("Cannot load Source Image File ") + fname;

		Img<int> img = Img<int>(buf, width, height);

		// .........................................................................
		// analyse the image and make adjustments, if desired

		Stat stat = {};
		analyse(img, stat);

		if(std::string("NST").find_first_of(lumaAlgo) == std::string::npos)
			throw "Unrecognised luma-algorithm flag(s) " + lumaAlgo;

		while(lumaAlgo != "N") {
			if(stat.unique <= 32)
				// too few unique colours, no point trying to improve the image
				break;

			if(lumaAlgo.find("S") != std::string::npos)
				if(255 - (stat.maxLuma - stat.minLuma) > 16) {
					stretch(img, stat);
					analyse(img, stat);	// update the stats
				}

			if(lumaAlgo.find("T") != std::string::npos)
				th = stat.avrLuma;

			break;
		}

		// if desired, write out GRAP file with the statistics
		if(ana)
			emitstats(ana, stat);

		// .........................................................................
		// now do the main thing: dither the image

		if(overTh >= 0x00 and overTh <= 0xff) {
			if(lumaAlgo.find("T") != std::string::npos)
				throw "Cannot use -t " + std::to_string(overTh) + " and -l T together";
			th = overTh;
		}

		dither(img, th, ditherAlgo);

		// .........................................................................

		// draw a border, if desired
		drawBorder(img, border);

		// write output data
		if(outName.rfind(".png", outName.length() - 4, 4) != std::string::npos)
			emitPng(outName, img);
		else
			emitPs(out, img);

		rc = 0;

	} catch(std::string& str) {
		printError(str);
	} catch(std::exception& e) {
		printError(e.what());
	} catch(...) {
		printError("Internal Error");
	}

	// clean up
	if(out)
		fclose(out);
	if(ana)
		fclose(ana);

	if(buf)
		delete[] buf;

	return rc;
}
