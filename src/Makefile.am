# use noinst here, so that it's a convenience library, which means also that PIC code is used.
# PIC code is needed for linking measuresuite-binding.c into measuresuite.node
noinst_LTLIBRARIES = libmeasuresuite.la
libmeasuresuite_la_SOURCES = checker.c \
							 checker.h \
							 evaluator.c \
							 evaluator.h \
							 measure_helper.c \
							 measure_helper.h \
							 measuresuite.c \
							 ms_error_description.c \
							 ms_error_description.h \
							 ms_error.h \
							 printer.c \
							 printer.h \
							 randomizer.c \
							 randomizer.h \
							 run_options.h \
							 timer.c \
							 timer.h
include_HEADERS = measuresuite.h

AM_CFLAGS = -Wall

clean-local:
	-rm -rf .libs .deps
