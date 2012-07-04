AC_INIT([MaxMind GeoIP dlfunc for Exim], [0.1], [snabb@epipe.com],
  [exim-geoipv6-dlfunc], [http://dist.epipe.com/exim/])

AC_CONFIG_SRCDIR([exim-geoipv6-dlfunc.c])
AC_CONFIG_HEADERS([config.h])
AC_CONFIG_FILES([Makefile])
AC_CONFIG_AUX_DIR([build-aux])
AC_CONFIG_MACRO_DIR([m4])

AM_INIT_AUTOMAKE([foreign])
LT_INIT

AC_PROG_CC
AC_PROG_CC_C99

AC_CHECK_HEADER([local_scan.h], [],
  [AC_MSG_ERROR([Required Exim local_scan.h include file was not found])])

AC_CHECK_HEADER([GeoIP.h], [],
  [AC_MSG_ERROR([MaxMind GeoIP C API GeoIP.h include file was not found])])
AC_CHECK_LIB([GeoIP], [GeoIP_country_code_by_addr], [],
  [AC_MSG_ERROR([MaxMind GeoIP C API library libGeoIP not found])])
AC_CHECK_LIB([GeoIP], [GeoIP_country_code_by_addr_v6], [],
  [AC_MSG_ERROR([No IPv6 support in libGeoIP (probably too old version])])

AC_OUTPUT
