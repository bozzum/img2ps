# This file is part of img2ps.
# Copyright (c) 2022-2023, Patric Keller (bozzumdesign at gmail dot com)
# The code is made available under the MIT License, see LICENSE document

{
	if($2 ~ /BUILD_NUM/)
		$3 = $3 + 1;
	print $0;
}
