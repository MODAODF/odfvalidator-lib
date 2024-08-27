/* -*- Mode: C++; tab-width: 4; indent-tabs-mode: nil; c-basic-offset: 4; fill-column: 100 -*- */
/*
 * SPDX-License-Identifier: MPL-2.0
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <ODFValidator.hpp>

#include <string>

ODFValidator::ODFValidator(std::string& file)
{
	_file = file;
}

std::string ODFValidator::check(std::string& file)
{
	_file = file;
	executeRealCommand();
	return _result;
}

void ODFValidator::executeRealCommand()
{
	std::string javaCmd = "java";
}

/* vim:set shiftwidth=4 softtabstop=4 expandtab: */
