
noinst_LIBRARIES = libwtf8tools.a
bin_PROGRAMS = wtf82wtf16 wtf162wtf8 wtf82wtf32 wtf322wtf8

libwtf8tools_a_SOURCES = \
	lib/verbose.c \
	lib/verbose.h \
	lib/wtf8.c \
	lib/wtf8.h \
	lib/wtf16.c \
	lib/wtf16.h \
	lib/wtf32.c \
	lib/wtf32.h

AM_CPPFLAGS = -I$(srcdir)/lib/

wtf82wtf16_SOURCES = wtf82wtf16.c
wtf82wtf16_LDADD = libwtf8tools.a

wtf162wtf8_SOURCES = wtf162wtf8.c
wtf162wtf8_LDADD = libwtf8tools.a

wtf82wtf32_SOURCES = wtf82wtf32.c
wtf82wtf32_LDADD = libwtf8tools.a

wtf322wtf8_SOURCES = wtf322wtf8.c
wtf322wtf8_LDADD = libwtf8tools.a
