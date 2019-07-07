#!/bin/sh

checkerr()
{
	if [ $? -ne 0 ]; then
		echo $*
		exit 1
	fi
}

rm -f Makefile
rm -f Makefile.in
rm -f aclocal.m4
rm -fr autom4te.cache/
rm -f compile
rm -f config.guess
rm -f config.h.in
rm -f config.sub
rm -f configure
rm -f depcomp
rm -f install-sh
rm -f missing
rm -f config.h f config.log f config.status
rm -fr src/.deps/
rm -f src/.dirstamp
rm -f stamp-h1
rm -f config.version

echo OK

