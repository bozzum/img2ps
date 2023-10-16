# img2ps -- B/W Dithering Tool (for  troff-based typesetting systems)
<p align="center"><img src="logo.jpg" /></p>

img2ps is a

## Usage
img2ps is a command-line tool. It takes a single image file (either in PNG or Jpeg
format), dithers the image, and emits it as a PostScript (PS) document, either to
stdout or to a file.
In addition, it can also produce a histogram of the luminance distribution in
form of a GRAP file - these files can be processed with the troff/groff tool
grap and then processed with the pic preprocessor.

The syntax is `img2ps [options] <img-file|::test>`.
For a full list of options use `img2ps -?`


## Building img2ps

### Linux

### Windows
Building img2ps on/for Windows should be easy, provided the right toolkit has been
installed. I do recommend MSYS2 (https://www.msys2.org/), but it should also be
possible to compile under MinGW. The (full) build process also needs awk and upx.
Personally, I prefer the original Kernighan awk version, but any compatible
awk will do as well. upx is a executable compression program and is available
for both Linux and Windows (https://upx.github.io/).

## Example
Here is an example of various dithering: [testDither.pdf](testDither.pdf).

## Future Features
* Scaling of Source Image -- Putting the scaler into the dithering tool would
allow us to optimise the scaled picture for dithering, rather than apply a generic
scaling that pleases the eye.
* PNG output -- Producing PNG output would open up the dithering tool to other
typesetting or documentation tools.

## Acknowledgements
img2ps uses the following 3rd party software:
* PNG decoder: lodepng (Version 20230410) by Lode Vandevenne.
https://lodev.org/lodepng/ https://github.com/lvandeve/lodepng (zlib License)
* JPEG decoder: jpgd (Release March 25, 2020 ) by Richard Geldreich.
https://github.com/richgel999/jpeg-compressor/tree/master ("Public Domain" license)

The build process requires:
* GNU gcc compiler suite (version 11 or above, C++17 support required)
* System tools like rm, mkdir, cp, awk...
* (optional): upx - file compressor (https://upx.github.io/)
* (optional) cppcheck -  static code analyser (http://cppcheck.net/)

## Reference
* Dithering: https://tannerhelland.com/2012/12/28/dithering-eleven-algorithms-source-code.html
* GRAP files: https://manpages.ubuntu.com/manpages/impish/en/man1/grap.1.html

## License
img2ps is made available under the [MIT License](LICENSE).
Copyright (c) 2023 Patric Keller
