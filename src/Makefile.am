## automake.am
##
## Makefile for the ttylogger(8) daemon
##
## (C) Copyright 2016 Fred Gleason <fredg@paravelsystems.com>
##
##   This program is free software; you can redistribute it and/or modify
##   it under the terms of the GNU General Public License as
##   published by the Free Software Foundation; either version 2 of
##   the License, or (at your option) any later version.
##
##   This program is distributed in the hope that it will be useful,
##   but WITHOUT ANY WARRANTY; without even the implied warranty of
##   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
##   GNU General Public License for more details.
##
##   You should have received a copy of the GNU General Public
##   License along with this program; if not, write to the Free Software
##   Foundation, Inc., 675 Mass Ave, Cambridge, MA 02139, USA.
##
## Use automake to process this into a Makefile.in

AM_CPPFLAGS = -Wall -DPREFIX=\"$(prefix)\" -Wno-strict-aliasing @QT4CLI_CFLAGS@
MOC = @QT4_MOC@

# The dependency for qt's Meta Object Compiler (moc)
moc_%.cpp:	%.h
	$(MOC) $< -o $@


sbin_PROGRAMS = ttylogger

dist_ttylogger_SOURCES = cmdswitch.cpp cmdswitch.h\
                         config.cpp config.h\
                         parser.cpp parser.h\
                         profile.cpp profile.h\
                         scriptevent.cpp scriptevent.h\
                         ttydevice.cpp ttydevice.h\
                         ttylogger.cpp ttylogger.h

nodist_ttylogger_SOURCES = moc_config.cpp\
                           moc_parser.cpp\
                           moc_scriptevent.cpp\
                           moc_ttydevice.cpp\
                           moc_ttylogger.cpp

ttylogger_LDADD = @QT4CLI_LIBS@

CLEANFILES = *~\
             moc_*\
             *.obj\
             *.idb\
             *.pdb\
             *ilk

MAINTAINERCLEANFILES = *~\
                       Makefile.in
