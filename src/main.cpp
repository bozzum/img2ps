
#include <getopt.h>
#include "loadfile.h"
#include "analyse.h"
#include "lumaproc.h"
#include "emitstats.h"
#include "dither.h"
#include "emitps.h"
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
		"\n -p num\t\tPixel Per Inch [default: 96]"
		"\n -s file\tStats file name [default: null]"
		"\n -t num\t\tOverwrite luma threshold level: 0..255 [default: -1], mainly for '-d TH'"
		"\n -v\t\tDisplay version info"
		"\n -?\t\tDisplay this help message"
	);

	fprintf(stderr, "\n\n"
		"Dither Algorithms:\n"
		"TH -- Threshold. This is the most simplest way of dithering, as it only\n"
		"has a single threshold for deciding between black and white. This algo-\n"
		"rithm has limited use, mainly for dithering text or drawings. Use the\n"
		"-t <val> option to optimize the threshold level\n"
		"FS -- Floyd-Steinberg. It was first published in 1976 by Robert W. \n"
		"Floyd and Louis Steinberg. It is an Error-diffusion dithering that \n"
		"diffuses the quantization error to neighboring pixels\n"
		"JJN -- Jarvis, Judice & Ninke. This algorithm tends to produce coarser\n"
		"patterns than the Floyd-Steinberg Dithering method, but with fewer\n"
		"artifacts\n"
		"SI -- Sierra. Similar to Jarvis algorithm\n"
		"\n"
		"Luma Algorithm Flags:\n"
		"N -- No Luma Threshold or Image Optimisation. This flag cannot be com-\n"
		"bined with any other luma flags.\n"
		"S -- Stretches the luma range of the image. This is useful for images\n"
		"with low contrast. This algorithm does not change the B/W threshold\n"
		"T -- Change the B/W threshold to the average luma value. This can be\n"
		"particularly effective if combines with the 'S' (stretch) algorithm\n"
		"\n"
		"Chroma to Luma Conversion Algorithm (PNG input only):\n"
		"C -- CCIR 601 (R: 0.299, G: 0.587, B: 0.114)\n"
		"B -- BT709 (R: 0.2126, G: 0.7152, B: 0.0722)\n"
		"S -- SMPTE 240M (R: 0.212, G: 0.701, B: 0.087)\n"
		"\n"
		"If a Stats file name has been specified, img2ps will output an additional\n"
		"GRAP file (Kernighan and Bentley's language for typesetting graphs),\n"
		"which can be used to produce a histogram display of the source image file\n"
		"\n"
		"The internal test pattern generator (\"::test\" as input file name) can\n"
		"be used to verify and calibrate the printer output. The shape should be\n"
		"a perfect circle within a perfect square\n"
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

int
main(int argc, char** argv)
{
	FILE* out = stdout;
	FILE* ana = nullptr;
	std::string lumaAlgo("ST");
	std::string chromaAlgo("C");
	std::string ditherAlgo("JJN");
	int ppi = 96;
	int th = 128;
	int border = 0;
	int overTh = -1;

	for(int o; (o = getopt(argc, argv, "?b:c:d:l:o:p:s:t:v")) != -1; )
		switch(o) {
			case '?': printUsage(); return 0;
			case 'b': border = (int)strtoul(optarg, nullptr, 0); break;
			case 'c': chromaAlgo = optarg; break;
			case 'd': ditherAlgo = optarg; break;
			case 'l': lumaAlgo = optarg; break;
			case 'o': out = fopen(optarg, "wb"); break;
			case 'p': ppi = (int)strtoul(optarg, nullptr, 0); break;
			case 's': ana = fopen(optarg, "wb"); break;
			case 't': overTh = (int)strtoul(optarg, nullptr, 0); break;
			case 'v': printVersion(); return 0;
		}

	if(optind == argc) {
		printUsage();
		return 1;
	}

	if(not out) {
		fprintf(stderr, "Cannot open output file -- Aborting..");
		return 2;
	}

	auto fname = argv[optind];

	// .........................................................................

	int width = 0, height = 0;
	auto buf = loadFile(fname, chromaAlgo, width, height, ppi);
	if(not buf) {
		fprintf(stderr, "Image decode error -- Aborting..");
		return 3;
	}

	Img<int> img = Img<int>(buf, width, height);

	// .........................................................................

	Stat stat = {};
	analyse(img, stat);

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

	if(ana)
		emitstats(ana, stat);

	// .........................................................................

	if(overTh >= 0x00 and overTh <= 0xff) {
		if(lumaAlgo.find("T") != std::string::npos)
			fprintf(stderr, "Cannot use -t %i and -l T together -- Aborting..", overTh);
		th = overTh;
	}

	if(dither(img, th, ditherAlgo)) {
		fprintf(stderr, "Unrecognised dither algorithm '%s' -- Aborting..", ditherAlgo.c_str());
		return 5;
	}

	// .........................................................................

	emitPs(out, img, ppi, border);

	if(out)
		fclose(out);
	if(ana)
		fclose(ana);

	if(buf)
		delete[] buf;

	return 0;
}
