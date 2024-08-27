# -*- Mode: makefile-gmake; tab-width: 4; indent-tabs-mode: t -*-
#
# SPDX-License-Identifier: MPL-2.0
#
# This Source Code Form is subject to the terms of the Mozilla Public
# License, v. 2.0. If a copy of the MPL was not distributed with this
# file, You can obtain one at http://mozilla.org/MPL/2.0/.
#


AM_CPPFLAGS = -pthread -I./include

if !ENABLE_DEBUG
AM_CPPFLAGS += -DNDEBUG
endif

AM_LDFLAGS = -pthread

odfvalidator_sources = \
    lib/ODFValidator.cpp

lib_LTLIBRARIES = libodfvalidator.la
libodfvalidator_la_SOURCES = $(odfvalidator_sources)

odfvalidator_includedir = $(includedir)/odfvalidator
odfvalidator_include_HEADERS = include/ODFValidator.hpp

# vim: set noet sw=4 ts=4:
