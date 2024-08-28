/* -*- Mode: C++; tab-width: 4; indent-tabs-mode: nil; c-basic-offset: 4; fill-column: 100 -*- */
/*
 * SPDX-License-Identifier: MPL-2.0
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#include <iostream>
#include <string>
#include <iostream>
#include <iomanip>

#include <ODFValidator.hpp>

int main(int argc, char *argv[])
{
    ODFValidator validator; // Create an instance of the ODFValidator class.

    // 沒有參數
    if (argc < 2)
    {
        std::cout << "Usage: " << argv[0] << " <file> [params]" << std::endl;
        std::cout << "Check if the file complies with ODF specifications." << std::endl;
        std::cout << "  <file>   The file to check." << std::endl;
        std::cout << "  [params] The parameters to pass to the validator." << std::endl;
        std::cout << "           Default parameters: -v -e" << std::endl;
        std::cout << "           -h, --help    Display detailed help." << std::endl;
        return 1;
    }

    // 第二個參數是 -h 或 --help
    if (std::string(argv[1]) == "-h" || std::string(argv[1]) == "--help")
    {
        validator.showHelp();
        return 1;
    }

    std::string externalParams = "";
    for (int i = 2; i < argc; i++)
    {
        externalParams += (i == 2 ? "" : " ") + std::string(argv[i]);
    }
    std::cout << "External parameters: " << externalParams << std::endl;

    // Set the parameters if they were provided
    if (!externalParams.empty())
        validator.setParams(externalParams);

    // Read the first command-line argument
    std::string filename = argv[1];

    // Call the check method
    validator.check(filename);

    // Get the result of the validation
    std::vector<std::string> result = validator.getResults();
    // print the result
    std::size_t lineNo = 0;
    for (const auto &line : result)
    {
        // 固定 5 位數
        std::cout << std::setw(5) << ++lineNo << ": " << line << std::endl;
    }

    return 0;
}

/* vim:set shiftwidth=4 softtabstop=4 expandtab: */
