/* -*- Mode: C++; tab-width: 4; indent-tabs-mode: nil; c-basic-offset: 4; fill-column: 100 -*- */
/*
 * SPDX-License-Identifier: MPL-2.0
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include "config.h"

#include <stdio.h>
#include <string>
#include <iostream>

#include <ODFValidator.hpp>

std::string ODFValidator::check(std::string& file)
{
	_file = file;
	executeRealCommand();
	return _result;
}

void ODFValidator::executeRealCommand()
{
	_result = ""; // Clear the result.

    std::string javaCmd = "java -jar " + std::string(ODFVALIDATOR_JAR_PATH) + " \"" + _file + "\" -v -e 2>&1";
	std::cout << "Executing command: " << javaCmd << std::endl;

	FILE *fd = popen(javaCmd.c_str(), "r");
	if (!fd)
	{
		std::cerr << "Error executing command: " << javaCmd << std::endl;
		return;
	}

	char buffer[128];
	while (fgets(buffer, sizeof(buffer), fd) != NULL)
	{
		_result += buffer;
	}

	int retcode = pclose(fd);
	std::cout << "Return code: " << retcode << std::endl;
}

/* vim:set shiftwidth=4 softtabstop=4 expandtab: */
