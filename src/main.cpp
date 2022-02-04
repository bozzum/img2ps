
#include <getopt.h>
#include "loadfile.h"
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
		"Usage: " APPNAME " [options] <img-file>\n"
		"\nImg-File:"
		"\n *.png\tPNG image"
		"\n *.jpg/jpeg\tJPEG image"
		"\n ::test\tinternale test pattern"
		"\nOptions:"
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

	int width = 0, height = 0;
	auto buf = loadFile(fname, chromaAlgo, width, height, ppi);
	if(not buf) {
		fprintf(stderr, "Image decode error -- Aborting..");
		return 3;
	}

	Img img = { buf, width, height };

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

	free(buf);

	return 0;
}
