AC_INIT([gdg], [1.0], [nipil@users.noreply.github.com], [gdg], [https://github.com/nipil/groovy-dsl-generator])
AM_INIT_AUTOMAKE([-Wall -Werror foreign])
AC_PROG_CXX
AC_CONFIG_HEADERS([config.h])
AC_CONFIG_FILES([
 Makefile
 src/Makefile

])

AC_PROG_LEX
AM_PROG_LEX
AC_PATH_PROG(LEX_INST, $LEX)
if test -z "$LEX_INST"; then
   AC_MSG_ERROR([flex/lex not found, please install any])
fi

AC_PROG_YACC
AC_PATH_PROG(YACC_INST, $YACC)
if test -z "$YACC_INST"; then
   AC_MSG_ERROR([bison/byacc/yacc not found, please install any])
fi

AC_OUTPUT
