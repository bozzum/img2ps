// This file is part of img2ps.
// Copyright (c) 2022-2023, Patric Keller (bozzumdesign at gmail dot com)
// The code is made available under the MIT License, see LICENSE document

#pragma once

#include "main.h"
#include "img.h"
#include <string>

int emitPng(const std::string& filename, Img<int>& img);
