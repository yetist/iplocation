NULL =
bin_PROGRAMS = iplocationd iplocation

common_CFLAGS = 	\
	-Wall		\
	-DIP_LOCATION_DIR=\""$(datadir)/iplocation"\"

iplocationd_CFLAGS =	\
	$(common_CFLAGS)	\
	$(GIO_CFLAGS)

iplocationd_LDADD =	\
	$(GIO_LIBS) 

iplocationd_SOURCES =	\
	$(location_built_sources)	\
	qqwry.c	\
	iplocationd.c	\
	$(NULL)

iplocation_CFLAGS =	\
	$(GIO_CFLAGS)

iplocation_LDADD =	\
	$(GIO_LIBS) 

iplocation_SOURCES =	\
	iplocation.c	\
	$(NULL)

location_built_sources =	\
	moses-ip-location-generated.h	\
	moses-ip-location-generated.c	\
	$(NULL)

$(location_built_sources) : Makefile.am $(srcdir)/org.moses.IpLocation.xml
	$(AM_V_GEN) gdbus-codegen \
	 	--interface-prefix org.moses. \
		--c-namespace Moses \
		--generate-c-code moses-ip-location-generated \
		$(srcdir)/org.moses.IpLocation.xml

EXTRA_DIST = 			\
	qqwry.h	\
	org.moses.IpLocation.xml	\
	$(NULL)
