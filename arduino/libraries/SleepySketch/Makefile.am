# $Id$

# This file is part of Citizen Sensing, sensor networking for the citizen scientist
# Copyright (c) 2013, the University Court of the University of St Andrews
#
# Citizen Sensing is free software; you can redistribute it and/or
# modify it under the terms of the GNU General Public License
# as published by the Free Software Foundation; either version 2
# of the License, or (at your option) any later version.
#
# Citizen Sensing is distributed in the hope that it will be useful,
# but WITHOUT ANY WARRANTY; without even the implied warranty of
# MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
# GNU General Public License for more details.
#
# You should have received a copy of the GNU General Public License
# along with this program; if not, write to the Free Software
# Foundation, Inc., 59 Temple Place, Suite 330, Boston, MA 02111, USA.

# ---------- Manifest ----------

# Library sources
src = \
	SleepySketch.h \
	SleepySketch.cpp \
	Actor.h \
	Actor.cpp \
	Sleeper.h \
	Sleeper.cpp \
	BusySleeper.h \
	BusySleeper.cpp \
	HeavySleeper.h \
	HeavySleeper.cpp \
	RepeatingActor.h \
	RepeatingActor.cpp

# Code examples
examples = \
	examples/blink/blink.ino

# Design documents and diagrams
design = \
	design/sleepysketch.uxf \
	design/sleepysketch.png \
	design/sleepysketch.svg \
	design/sleepysketch.pdf

# Miscellaneous files, not distributed
misc = \
	Makefile \
	Doxyfile


# ---------- Extra targets ----------

doc: $(src) $(design)
	$(DOXYGEN) 
