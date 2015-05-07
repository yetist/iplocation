datdir = $(datadir)/iplocation
dat_DATA = qqwry.dat

data_servicedir = $(datadir)/dbus-1/services
data_service_in_files = org.moses.iplocation.service.in
data_service_DATA = $(data_service_in_files:.service.in=.service)

$(data_service_DATA): $(data_service_in_files) Makefile
	@sed -e "s|\@bindir\@|$(bindir)|" $<> $@

EXTRA_DIST =					\
	$(dat_DATA)	\
	$(data_service_in_files)

CLEANFILES =					\
	$(data_service_DATA)

-include $(top_srcdir)/git.mk
