NULL =
bin_PROGRAMS = iplocationd

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
	main.c	\
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
	$(NULL)
