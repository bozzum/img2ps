{
	if($2 ~ /BUILD_NUM/)
		$3 = $3 + 1;
	print $0;
}
