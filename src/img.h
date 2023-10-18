// This file is part of img2ps.
// Copyright (c) 2022-2023, Patric Keller (bozzumdesign at gmail dot com)
// The code is made available under the MIT License, see LICENSE document

#pragma once

#include <algorithm>
#include <cstdint>
#include <cstdlib>

#include <cstdio>

typedef uint8_t		PxVal;
typedef uint16_t	Pattern;
typedef enum {
	ColWhite	= 0xff,
	ColLghtGrey	= 0xcc,
	ColGrey		= 0x80,
	ColDarkGrey	= 0x44,
	ColBlack	= 0x00
}					Colour;

// Class Pt
class Pt {
public:
	int		x;
	int		y;

			Pt(int xPos, int yPos) : x(xPos), y(yPos) { return; }
};

// Class Img
template<typename T>
class Img {
public:
	T*			data	{nullptr};
	int			w		{0};
	int			h		{0};

				Img() { return; }
	explicit	Img(T* ptrBuf, int width, int height) : data(ptrBuf), w(width), h(height) {
					area = w * h;
				}
	explicit	Img(int width, int height, PxVal fillVal = ColWhite) : w(width), h(height) {
					area = w * h;
					data = new T[area];
					set(fillVal);
					alloc = true;
				}
				Img(const Img&) = delete;

			   ~Img() {
					if(data and alloc)
						delete[] data;
				}

	Img&	operator=(const Img&) = delete;

			// Method: set
			// Sets the entire image to fillVal gray value.
			//
			// Parameters:
			//   fillVal -- Colour (gray) fill value
			//
	void	set(PxVal fillVal = ColWhite) {
				if(data)
					for(auto n = area; n--; )
						data[n] = static_cast<T>(fillVal);
			}

			// Method: pix
			// Draws a pixel.
			//
			// Parameters:
			//   p -- Location of the pixel
			//   v -- Colour (gray) value of the pixel
			//
	void	pix(Pt p, PxVal v) {
				if(data and p.x >= 0 and p.x < w and p.y >= 0 and p.y < h)
					data[p.x + (p.y * w)] = static_cast<T>(v);
			}

			// Method: line
			// Draws a line. Method uses the "midpoint algorithm", as developed
			// by PITTEWAY and  VAN AKEN, based on the BRESENHAM method.
			//
			// Parameters:
			//   p0 -- Start-point of the line
			//   p1 -- End-point of the line
			//   val -- Colour (gray) value of the line
			//   pat -- line pattern
			//
	void	line(Pt p0, Pt p1, PxVal val = ColBlack, Pattern pat = 0xffff) {
				if(p0.x > p1.x) {
					// points need to be in canonical order horizontally.
					auto t = p0;
					p0 = p1; p1 = t;
				}

				int dx = p1.x - p0.x;
				int dy, sy;
				int inc1, inc2;
				int d;
				Pt p = p0;

				if(p1.y > p0.y) {
					sy = 1;
					dy = p1.y - p0.y;
				} else {
					sy = -1;
					dy = p0.y - p1.y;
				}

				if(dx >= dy) {
					inc1 = dy << 1;
					d = inc1 - dx;
					inc2 = (dy - dx) << 1;

					for( ; p.x != p1.x; p.x++) {
						if(pat & 1)
							pix(p, val);

						if(d <= 0)
							d += inc1;
						else {
							d += inc2;
							p.y += sy;
						}
						pat = (pat >> 1) | (pat << 15);
					}
				} else {
					inc1 = dx << 1;
					d = inc1 - dy;
					inc2 = (dx - dy) << 1;

					for( ; p.y != p1.y; p.y += sy) {
						if(pat & 1)
							pix(p, val);

						if(d <= 0)
							d += inc1;
						else {
							d += inc2;
							p.x++;
						}
						pat = (pat >> 1) | (pat << 15);
					}
				}
				if(pat & 1)
					pix(p1, val);
			}

			// Method: rect
			// Draws a rectangle.
			//
			// Parameters:
			//   p0 -- Top-left coordinate of the rectangle
			//   p1 -- Bottom-right coordinate of the rectangle
			//   val -- Line colour (gray scale)
			//   doFill -- if true, draws a filled rectangle
			//
			// Note:
			// Coordinates can be swapped.
			//
	void	rect(Pt p0, Pt p1, PxVal val = ColBlack, bool doFill = false) {
				if(p0.x > p1.x)
					std::swap(p0.x, p1.x);
				if(p0.y > p1.y)
					std::swap(p0.y, p1.y);

				line(p0, Pt(p1.x, p0.y), val);

				if(doFill) {
					for(auto y = p0.y +1; y < p1.y; y++)
						line(Pt(p0.x, y), Pt(p1.x, y), val);
				} else {
					line(p0, Pt(p0.x, p1.y), val);
					line(Pt(p1.x, p0.y), p1, val);
				}

				line(Pt(p0.x, p1.y), p1, val);
			}

			// Method: ellipse
			// Draws an ellipse. The implementation follows a suggestion of
			// M. Douglas McIlroy ("There Is No Royal Road to Programs /
			// A Trilogy on Raster Ellipses and Programming Methodology",
			// AT&T Bell Labs).
			//
			// Parameters:
			//   org -- Top-left corner of enclosing box
			//   dimX -- Width of the enclosing box
			//   dimY --  Height of the enclosing box
			//   val -- Pixel gray value of the line
			//   doFill -- if true, creates a filled ellipse
			//
	void	ellipse(Pt org, int dimX, int dimY, PxVal val = ColBlack, bool doFill = false) {
				int sqrW  = dimX * dimX;
				int sqrH  = dimY * dimY;
				int crit1 = -(sqrW >> 2) + (dimX & 1) + sqrH;
				int crit2 = -(sqrH >> 2) + (dimY & 1) + sqrW;
				int crit3 = -(sqrH >> 2) + (dimY & 1);
				int t     = -sqrW * dimY;
				int d2xt  = sqrH << 1;
				int d2yt  = sqrW << 1;
				int dxt   = 0;
				int dyt   = d2yt * -dimY;
				int xrel  = 0;

				while(dimY >= 0 and xrel <= dimX) {
					if(doFill) {
						line(Pt(org.x + xrel, org.y - dimY), Pt(org.x + xrel, org.y + dimY), val);
						line(Pt(org.x - xrel, org.y - dimY), Pt(org.x - xrel, org.y + dimY), val);
					} else {
						pix(Pt(org.x + xrel, org.y + dimY), val);
						if(xrel or dimY)
							pix(Pt(org.x - xrel, org.y - dimY), val);
						if(xrel and dimY) {
							pix(Pt(org.x + xrel, org.y - dimY), val);
							pix(Pt(org.x - xrel, org.y + dimY), val);
						}
					}

					if((t + sqrH * xrel <= crit1) or (t + sqrW * dimY <= crit3)) {
						xrel++;
						dxt += d2xt;
						t   += dxt;
					}
					else if(t - sqrW * dimY > crit2) {
						dimY--;
						dyt += d2yt;
						t   += dyt;
					}
					else {
						xrel++;
						dxt += d2xt;
						t   += dxt;
						dimY--;
						dyt += d2yt;
						t   += dyt;
					}
				}
			}


private:
	int		area	{0};
	bool	alloc	{false};
};
