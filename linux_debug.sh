#!/bin/sh
file="build/Makefile.linux_debug"

checkerr()
{
	if [ $? -ne 0 ]; then
		echo $*
		exit 1
	fi
}

cp -f ${file} Makefile
checkerr "cp Makefile error"

make -f ${file} clean
checkerr "make clean error";

make -f ${file} depend
checkerr "make depend error";

make -f ${file} build ${@}
checkerr "make build error";

echo OK

