win:
	rm -f build/_lgb*
	gcc \
		-Isqlite-autoconf-3420000/ \
		-Llib_lightgbm.dll \
		-o build/_lgb.dll \
		-shared \
		sqlmath_wrapper_lightgbm.c

	printf ".load build/_lgb.dll" > build/_lgb_init.sql

	./_sqlmath.shell_win32_x64.exe \
		":memory:" \
		-init build/_lgb_init.sql \
		"SELECT lgb_hello();"
	echo $?

