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

    // No arguments provided.
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

    // Display detailed help.
    if (std::string(argv[1]) == "-h" || std::string(argv[1]) == "--help")
    {
        validator.showHelp();
        return 1;
    }

    // Read the external parameters
    std::string externalParams = "";
    for (int i = 2; i < argc; i++)
    {
        externalParams += (i == 2 ? "" : " ") + std::string(argv[i]);
    }

    // Set the parameters if they were provided
    if (!externalParams.empty())
    {
        std::cout << "External parameters: " << externalParams << std::endl;
        validator.setParams(externalParams);
    }

    // Read the first command-line argument
    std::string filename = argv[1];

    // Call the check method
    validator.check(filename);

    // Get return code
    int returnCode = validator.getReturnCode();
    if (returnCode != 0)
    {
        // 輸出亮紅色 Error: 錯誤信息
        std::cerr << "\033[1;31mError: " << "\033[0m" << validator.getError() << std::endl;
        return returnCode;
    }

    // 輸出淺綠色 JSON 結果
    std::cout << "\033[32m" << validator.getJsonResult() << "\033[0m" << std::endl;

    return 0;
}

/* vim:set shiftwidth=4 softtabstop=4 expandtab: */
