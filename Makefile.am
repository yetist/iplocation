SUBDIRS = src data

dist-sign: dist-xz
	gpg --detach-sign --use-agent $(distdir).tar.xz

upload: dist-sign
	scp $(distdir).tar.xz $(distdir).tar.xz.sig ftp.isoft.zhcn.cc:/srv/ftp/other/moses-iplocation/

.PHONY: dist-sign upload

-include $(top_srcdir)/git.mk
