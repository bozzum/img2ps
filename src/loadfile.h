// This file is part of img2ps.
// Copyright (c) 2022-2023, Patric Keller (bozzumdesign at gmail dot com)
// The code is made available under the MIT License, see LICENSE document

#pragma once

#include <string>
#include "main.h"

int* loadFile(const std::string& path, std::string& algo, int& width, int& height, int ppi);
