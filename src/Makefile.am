##   main.c - A simple hello-world program.
##
##   Copyright (c) 2014, Francisco Jose Monaco  <moanco@icmc.usp.br>
##
##   This file is part of POSIXeg
##
##   POSIXeg is free software: you can redistribute it and/or modify
##   it under the terms of the GNU General Public License as published by
##   the Free Software Foundation, either version 3 of the License, or
##   (at your option) any later version.
##
##   This program is distributed in the hope that it will be useful,
##   but WITHOUT ANY WARRANTY; without even the implied warranty of
##   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
##   GNU General Public License for more details.
##
##   You should have received a copy of the GNU General Public License
##   along with this program.  If not, see <http://www.gnu.org/licenses/>.

AM_CPPFLAGS = -I$(top_srcdir)/lib  @CPP_FLAGS@
AM_CFLAGS =   @C_FLAGS@ 
AM_LDFLAGS =  -I$(top_srcdir)/lib  @LD_FLAGS@ 


bin_PROGRAMS = asciiplay forkn

asciiplay_SOURCES = asciiplay.c utils.c utils.h 

asciiplay_CC = @PTHREAD_CC@
asciiplay_CFLAGS = @PTHREAD_CFLAGS@ 
asciiplay_LDADD = -lncurses $(LIBOBJS) @PTHREAD_LIBS@ ../lib/libasciiplay.la

forkn_SOURCES = forkn.c



