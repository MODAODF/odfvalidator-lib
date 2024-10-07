# -*- Mode: makefile-gmake; tab-width: 4; indent-tabs-mode: t -*-
#
# This Source Code Form is subject to the terms of the Mozilla Public
# License, v. 2.0. If a copy of the MPL was not distributed with this
# file, You can obtain one at http://mozilla.org/MPL/2.0/.
#

SUBDIRS = src resources

bin_PROGRAMS = odfvalidator odfchecker

odfvalidator_CPPFLAGS = -pthread -I$(abs_top_builddir)/src/include
odfvalidator_SOURCES = tools/odfvalidator.cpp
odfvalidator_LDADD = $(abs_top_builddir)/src/libodfvalidator.la

odfchecker_CPPFLAGS = -pthread -I$(abs_top_builddir)/src/include
odfchecker_SOURCES = tools/odfchecker.cpp
odfchecker_LDADD = $(abs_top_builddir)/src/libodfvalidator.la

csharpexampledir = $(datadir)/odfvalidator/example/C-Sharp/TestSo
csharpexample_DATA = example/Linux/C-Sharp/TestSo/Program.cs \
                     example/Linux/C-Sharp/TestSo/TestSo.csproj \
                     example/Linux/C-Sharp/TestSo/TestSo.generated.sln

phpexampledir = $(datadir)/odfvalidator/example/PHP
phpexample_DATA = example/Linux/PHP/testSo.php

EXTRA_DIST = odfvalidator.spec

# vim: set noet sw=4 ts=4:
