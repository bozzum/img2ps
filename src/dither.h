// This file is part of img2ps.
// Copyright (c) 2022-2023, Patric Keller
// The code is made available under the MIT License, see LICENSE document

#pragma once

#include <string>
#include "img.h"
#include "main.h"

int dither(Img<int>& img, int th, const std::string& algo);
