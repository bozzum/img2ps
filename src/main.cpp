
#include <getopt.h>
#include "lodepng.h"
#include "chromaproc.h"
#include "analyse.h"
#include "lumaproc.h"
#include "emitstats.h"
#include "dither.h"
#include "emitps.h"
#include "main.h"

static void
printUsage()
{
	fprintf(stderr,
		"Usage: " APPNAME " [options] <png-file>\n"
		"\n -b num\tBorder width [default: 0]"
		"\n -c algo\tChroma algorithm: I [default: I]"
		"\n -d algo\tDither algorithm: TH, FS, JJN, SI [default: JJN]"
		"\n -l algo\tLuma algorithm: N, S, T [default: ST]"
		"\n -o file\tOutput file name [default: stdout]"
		"\n -p num\tPixel Per Inch [default: 96]"
		"\n -s file\tStats file name [default: null]"
		"\n -?\tDisplay this help message"
	);
}

int
main(int argc, char** argv)
{
	FILE* out = stdout;
	FILE* ana = nullptr;
	std::string lumaAlgo("ST");
	std::string chromaAlgo("I");
	std::string ditherAlgo("JJN");
	int ppi = 96;
	int th = 128;
	int border = 0;

	for(int o; (o = getopt(argc, argv, "?b:c:d:l:o:p:s:")) != -1; )
		switch(o) {
			case '?': printUsage(); return 0;
			case 'b': border = (int)strtoul(optarg, nullptr, 0); break;
			case 'c': chromaAlgo = optarg; break;
			case 'd': ditherAlgo = optarg; break;
			case 'l': lumaAlgo = optarg; break;
			case 'o': out = fopen(optarg, "wb"); break;
			case 'p': ppi = (int)strtoul(optarg, nullptr, 0); break;
			case 's': ana = fopen(optarg, "wb"); break;
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

	std::vector<unsigned char> image;
	unsigned width, height;

	auto rc = lodepng::decode(image, width, height, fname);
	if(rc) {
		fprintf(stderr, "PNG decode error: %s -- Aborting..", lodepng_error_text(rc));
		return 3;
	}

	// .........................................................................

	Img img = { new int[width * height], (int)width, (int)height };

	// Chroma to luma conversion
	if(chromaConv(image.data(), img, chromaAlgo)) {
		fprintf(stderr, "Unrecognised chroma algorithm '%s' -- Aborting..", chromaAlgo.c_str());
		return 4;
	}

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

	if(dither(img, th, ditherAlgo)) {
		fprintf(stderr, "Unrecognised dither algorithm '%s' -- Aborting..", ditherAlgo.c_str());
		return 5;
	}

	// .........................................................................

	emitPs(out, img, ppi, border);

	return 0;
}
