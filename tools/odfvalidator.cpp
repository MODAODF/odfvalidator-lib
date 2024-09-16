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
        std::cout << "           -j, --json    Output the result in JSON format." << std::endl;
        std::cout << "           -h, --help    Display detailed help." << std::endl;
        return 1;
    }

    // Read the external parameters
    bool showHelp = false;
    bool outputJson = false;
    std::string externalParams = "";
    for (int i = 2; i < argc; i++)
    {
        if (std::string(argv[i]) == "-h" || std::string(argv[i]) == "--help")
            showHelp = true;
        else if (std::string(argv[i]) == "-j" || std::string(argv[i]) == "--json")
            outputJson = true;
        else
            externalParams += (i == 2 ? "" : " ") + std::string(argv[i]);
    }

    // Show detailed help
    if (showHelp)
    {
        validator.showHelp();
        return 0;
    }

    // Set the parameters if they were provided
    if (!externalParams.empty())
        validator.setParams(externalParams);

    // Read the first command-line argument
    std::string filename = argv[1];

    // Call the check method
    validator.check(filename);

    // Get return code
    int returnCode = validator.getReturnCode();
   if (outputJson)
        std::cout << validator.getJsonResult() << std::endl;
    else
    {
        std::cout << "Success: " << (returnCode == 0 ? "Yes" : "No") << std::endl;
        if (returnCode != 0)
        {
            std::cout << "Reason: " << validator.getError() << std::endl;
        }
        else
        {
            std::cout << "Validation: " << (validator.isValidation() ? "Yes" : "No") << std::endl;
            std::cout << "ODF Version: " << validator.getODFVersion() << std::endl;
            std::cout << "Generator: " << validator.getGenerator() << std::endl;
            if (!validator.getErrorMessages().empty())
            {
                std::cout << "Error Messages:" << std::endl;
                for (const auto& message : validator.getErrorMessages())
                {
                    std::cout << message << std::endl;
                }
            }
        }
    }

    return 0;
}

/* vim:set shiftwidth=4 softtabstop=4 expandtab: */
