MaxMind GeoIP dlfunc for Exim
=============================

This is an IPv4 and IPv6 capable GeoIP dlfunc library for Exim. It
implements an interface between Exim access control lists and MaxMind's
GeoIP database.  This can be useful for greylisting or scoring IP addresses
of SMTP senders according to the country code of the sender's IP address.

- Download:	https://dist.epipe.com/exim/
- GitHub:	https://github.com/snabb/exim-geoipv6-dlfunc
- Author:	Janne Snabb, snabb at epipe.com
- License:	LGPL version 2.1 or later


## Installation

The build system is based on GNU autoconf, automake and libtool. That
makes the size of this software tarball enormously big, but it is
supposedly the best somewhat portable way for creating shared libraries
without getting a headache from thinking about compiler and linker flags.

Exim's local_scan.h header file is needed for compilation. Also MaxMind's
GeoIP C API library is needed, version 1.4.7 or newer. On Debian and
Ubuntu the dependencies are available in `exim4-dev` and `libgeoip-dev`
packages:
```
apt-get install exim4-dev libgeoip-dev
```
Alternatively you may point the include path in CPPFLAGS to
some other directory where Exim's local_scan.h is located (such as Exim
build directory).

RHEL/CentOS/etc. have the GeoIP library in `GeoIP-devel` package.

You may want to alter some settings at the start of exim-geoipv6-dlfunc.c
to suit your local needs.

The following command can be used to configure the library on Debian
and Ubuntu Linux:
```
CPPFLAGS="-I/usr/include/exim4" ./configure --libdir=/usr/local/lib/exim4/
```

On systems which have the GeoIP library in /usr/local, such as FreeBSD,
you can specify something like the following:
```
CPPFLAGS="-I/usr/local/include -I/exim/build/directory" \
LDFLAGS="-L/usr/local/lib" \
./configure --libdir=/usr/local/lib/exim4/
```
(The previous command should be all on one single line if entered without
the backslashes.)

Compilation and installation should be as simple as the following if
the configure script was run with correct parameters and dependencies
are available:
```
make
make install
```

## Usage

Exim must be compiled with the "dlfunc" feature enabled (`EXPAND_DLFUNC=yes`).

Also Exim must be compiled with `-export-dynamic` in `EXTRALIBS` to make
Exim's local_scan API available.

On Debian and Ubuntu you need to use `exim4-daemon-heavy` package.
`exim4-daemon-light` package will *not* work.

You need to have MaxMind's GeoIP database available in the correct
location. IPv4 and IPv6 databases are in separate files. The Lite version
of the database is available free of charge at MaxMind web site. On Debian
and Ubuntu it is available in `geoip-database` or `geoip-database-contrib`
package:
```
apt-get install geoip-database-contrib
```

You can add something such as the following in Exim connect ACL:
```
warn    set acl_c_geoip_country_code = \
                ${dlfunc{/usr/local/lib/exim4/exim-geoipv6-dlfunc.so}\
                        {geoip_country_code}{$sender_host_address}}
```

After that you can use $acl_c_geoip_country_code variable in ACL
conditions, for example:
```
deny    condition = ${if inlist{$acl_c_geoip_country_code}{US:GB}}
        message = We do not accept messages from your country.
```

If you are using Exim version older than 4.77 you need to use "forany"
instead of "inlist" if you want to match against a list of country codes:
```
warn    condition = ${if forany{US:GB}\
                {eq{$item}{$acl_c_geoip_country_code}}}
        set acl_c_ipscore = ${eval:$acl_c_ipscore+10}
```

You can also add something like the following in the DATA ACL to add a
message header which indicates the country code of the connecting IP:
```
warn    condition = ${if def:acl_c_geoip_country_code}
        add_header = X-GeoIP: $acl_c_geoip_country_code
```

## Working with development version

If you check out the development version from GitHub, you need to have
GNU autotools, libtool, etc. installed.

To generate all the automatically created files you need to run the
`bootstrap` script.

