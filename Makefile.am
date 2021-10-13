SUBDIRS = src src-ts test
EXTRA_DIST = autogen.sh
ACLOCAL_AMFLAGS = -I m4 --install
AM_CFLAGS = -Wall -Wextra
NPM=npm

./node_modules/node-gyp/bin/node-gyp.js:
	/usr/bin/env $(NPM) install

all-local: ./node_modules/node-gyp/bin/node-gyp.js
	mkdir -p src-ts/lib src-ts/include
	cp src/.libs/libmeasuresuite.* src-ts/lib
	cp src/measuresuite.h src-ts/include
	cd ./src-ts && ../node_modules/node-gyp/bin/node-gyp.js rebuild

clean-local:
	-rm -rf dist/

check-local:
	mkdir -p ./dist
	cp test/libcheckfunctions-clang-AMD.so ./dist
	-$(NPM) run test

DISTCLEANFILES = ./config.h~ \
				 configure~

distclean-local:
	-rm -rf autom4te.cache/ \
		build/ \
		node_modules/
