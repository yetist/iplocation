aclocal
autoheader
autoconf
#intltoolize --force --copy --automake
#libtoolize -c
automake -a -c
./configure --enable-maintainer-mode --prefix=/usr
