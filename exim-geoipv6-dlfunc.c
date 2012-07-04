/*
 * exim-geoipv6-dlfunc.c - MaxMind GeoIP dlfunc for Exim
 *
 * Copyright (C) 2012 Janne Snabb <snabb@epipe.com>
 *
 * This library is free software; you can redistribute it and/or modify it
 * under the terms of the GNU Lesser General Public License as published
 * by the Free Software Foundation; either version 2.1 of the License, or
 * (at your option) any later version.
 *
 * This library is distributed in the hope that it will be useful, but
 * WITHOUT ANY WARRANTY; without even the implied warranty of MERCHANTABILITY
 * or FITNESS FOR A PARTICULAR PURPOSE. See the GNU Lesser General Public
 * License for more details.
 *
 * You should have received a copy of the GNU Lesser General Public License
 * along with this library; if not, write to the Free Software Foundation,
 * Inc., 51 Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
 */

#ifdef HAVE_CONFIG_H
#include "config.h"
#endif

/* Headers for inet_pton(3): */
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>

/* MaxMind GeoIP C API header: */
#include "GeoIP.h"

/* Exim4 dlfunc API header: */
#include "local_scan.h"

/*****************************************************************************
 * Configuration settings:
 *****************************************************************************/

/* GeoIP database edition selection: */
#define EDITION_V4	GEOIP_COUNTRY_EDITION
#define EDITION_V6	GEOIP_COUNTRY_EDITION_V6

/* default code returned when country is unknown: */
#define COUNTRY_CODE_UNKNOWN		US"--"

/* default code returned on lookup failures due to missing database: */
#define COUNTRY_CODE_LOOKUP_FAILED	US"--"

/*****************************************************************************
 * Country code lookup function:
 *****************************************************************************/

int
geoip_country_code(uschar **yield, int argc, uschar *argv[])
{
	GeoIP *gi;
	const char *res;
	union {
		struct in_addr buf4;
		struct in6_addr buf6;
	} buf;

	if (argc != 1) {
		*yield = string_copy(US"Invalid number of arguments");
                return ERROR;
        }

	if (inet_pton(AF_INET, (char *) argv[0], &buf) == 1) {
		/* IPv4 address: */

		gi = GeoIP_open_type(EDITION_V4, GEOIP_STANDARD);

		if (gi == NULL) {
			log_write(0, LOG_MAIN, US"geoipv6: Failed to open"
				 " IPv4 GeoIP database");
			*yield = string_copy(COUNTRY_CODE_LOOKUP_FAILED);
			return OK;
		}

		res = GeoIP_country_code_by_addr(gi, (char *) argv[0]);

	} else if (inet_pton(AF_INET6, (char *) argv[0], &buf) == 1) {
		/* IPv6 address: */

		gi = GeoIP_open_type(EDITION_V6, GEOIP_STANDARD);

		if (gi == NULL) {
			log_write(0, LOG_MAIN, US"geoipv6: Failed to open"
				 " IPv6 GeoIP database");
			*yield = string_copy(COUNTRY_CODE_LOOKUP_FAILED);
			return OK;
		}

		res = GeoIP_country_code_by_addr_v6(gi, (char *) argv[0]);

	} else {
		/* Unrecognized address format: */

		*yield = string_copy(US"Unrecognized address format");
		return FAIL;
	}
	GeoIP_delete(gi);

	if (res == NULL)
		*yield = string_copy(COUNTRY_CODE_UNKNOWN);
	else
		*yield = string_copy((uschar *) res);

	return OK;
}

/*****************************************************************************
 * eof
 *****************************************************************************/
