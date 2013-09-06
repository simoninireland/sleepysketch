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

# ---------- Autoconf and Automake magic ----------

AC_INIT([SleepySketch], [0.1],
        [info@citizen-sensing.org],
        [SleepySketch],
        [http://www.citizen-sensing.org])
AC_PREREQ(2.54)
AM_INIT_AUTOMAKE


# ---------- Basic programs ----------

# Basic elements
AC_SUBST(PACKAGE_NAME)
AC_SUBST(PACKAGE_VERSION)

# Bootstrapping 
AC_CHECK_PROG(DOXYGEN,doxygen,`which doxygen`)
test -z $DOXYGEN && AC_MSG_ERROR([SleepySketch uses dosygen to generate documentation])


# ---------- Create files ----------

AC_CONFIG_FILES([
Makefile
])

AC_OUTPUT

