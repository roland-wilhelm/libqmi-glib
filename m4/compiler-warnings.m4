AC_DEFUN([LIBQMI_GLIB_COMPILER_WARNINGS],
[AC_ARG_ENABLE(more-warnings,
	AS_HELP_STRING([--enable-more-warnings], [Maximum compiler warnings]),
	set_more_warnings="$enableval",set_more_warnings=yes)
AC_MSG_CHECKING(for more warnings, including -Werror)
if test "$GCC" = "yes" -a "$set_more_warnings" != "no"; then
	AC_MSG_RESULT(yes)
	CFLAGS="-Wall -Werror -std=gnu89 $CFLAGS"

	for option in -Wmissing-declarations -Wmissing-prototypes \
		      -Wdeclaration-after-statement -Wstrict-prototypes \
		      -fno-strict-aliasing -Wno-deprecated-declarations \
		      -Wint-to-pointer-cast; do
		SAVE_CFLAGS="$CFLAGS"
		CFLAGS="$CFLAGS $option"
		AC_MSG_CHECKING([whether gcc understands $option])
		AC_TRY_COMPILE([], [],
			has_option=yes,
			has_option=no,)
		if test $has_option = no; then
			CFLAGS="$SAVE_CFLAGS"
		fi
		AC_MSG_RESULT($has_option)
		unset has_option
		unset SAVE_CFLAGS
	done
	unset option
else
	AC_MSG_RESULT(no)
fi
])
