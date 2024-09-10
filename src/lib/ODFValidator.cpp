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
#include <sstream>
#include <unistd.h>

#include <ODFValidator.hpp>

ODFValidator::ODFValidator()
{
    _params = "-v -e"; // Default parameters.
}

void ODFValidator::showHelp()
{
    // Execute java -jar odfvalidator-0.12.0-jar-with-dependencies.jar -h
    std::string javaCmd("java -jar ");
    javaCmd.append(ODFVALIDATOR_JAR_PATH);
    javaCmd.append(" -h");
    FILE *fd = popen(javaCmd.c_str(), "r");
    if (fd)
    {
        char buffer[128];
        while (fgets(buffer, sizeof(buffer), fd) != NULL)
        {
            std::cout << buffer;
        }

        pclose(fd);
    }
    else
    {
        std::cout << "Error executing command: " << javaCmd << std::endl;
    }
}

void ODFValidator::check(std::string &file)
{
    _file = file;
    // TODO: 1.Check if the file exists.
    if (access(_file.c_str(), F_OK) == -1)
    {
        makeJsonResult(ErrorCode::FILE_NOT_FOUND);
        return;
    }

    // TODO: 2.Check if the file is an ODF file.
    if (!isZipFile(_file) || !isODFFile(_file))
    {
        makeJsonResult(ErrorCode::FILE_NOT_ODF);
        return;
    }

    // TODO: 3.Check if the file is a valid ODF file.
    executeRealCommand();
}

bool ODFValidator::isZipFile(const std::string &file)
{
    // First 4 bytes of a ZIP file are: 0x50 0x4B 0x03 0x04
    FILE *f = fopen(file.c_str(), "rb");
    if (f)
    {
        char buffer[4];
        if (fread(buffer, 1, 4, f) == 4)
        {
            if (buffer[0] == 'P' && buffer[1] == 'K' && buffer[2] == 3 && buffer[3] == 4)
            {
                fclose(f);
                return true;
            }
        }
        fclose(f);
    }

    return false;
}

bool ODFValidator::isODFFile(const std::string &file)
{
    // check file extension is .odt, .ods, .odp, .odg, .ott, .ots, .otp, .otg
    const std::string ext = file.substr(file.find_last_of(".") + 1);
    if (ext == "odt" || ext == "ods" || ext == "odp" || ext == "odg" ||
        ext == "ott" || ext == "ots" || ext == "otp" || ext == "otg")
    {
        return true;
    }
    return false;
}

void ODFValidator::executeRealCommand()
{
    _result.clear(); // Clear the result.
    _results.clear(); // Clear the results.
    _generator.clear(); // Clear the generator.

    std::string javaCmd("java");
    javaCmd.append(" -jar ");
    javaCmd.append(ODFVALIDATOR_JAR_PATH);
    javaCmd.append(" " + _params + " ");
    javaCmd.append("\"" + _file + "\"");
    javaCmd.append(" 2>&1");

    // Execute the command.
    FILE *fd = popen(javaCmd.c_str(), "r");
    if (fd)
    {
        char buffer[128];
        while (fgets(buffer, sizeof(buffer), fd) != NULL)
        {
            _result += buffer;
        }

        _returnCode = pclose(fd);

        // Convert result split by '\n' to vector.
        std::istringstream iss(_result);
        std::string line;
        while (std::getline(iss, line, '\n'))
        {
            _results.push_back(line); // Save the results.

            // Parse "Error: The ODF package " and " shell contain the "
            const std::size_t foundError1 = line.find("Error: The ODF package ");
            const std::size_t foundError2 = line.find(" shell contain the ");
            if (foundError1 != std::string::npos || foundError2 != std::string::npos)
            {
                makeJsonResult(ErrorCode::FILE_NOT_ODF);
                return;
            }

            // Parse "Error: " to get the validation result.
            const std::size_t foundError = line.find("Error: ");
            if (foundError != std::string::npos)
            {
                _validation = false;
                break;
            }
            else
            {
                _validation = true;
            }

            // Parse "Generator: " to get the last editor tool.
            const std::size_t foundGenerator = line.find("Generator: ");
            if (foundGenerator != std::string::npos)
            {
                std::size_t startPos = foundGenerator + 11;
                // find first space after "Generator: "
                std::size_t endPos = line.find(" ", startPos);
                if (endPos != std::string::npos)
                {
                    _generator = line.substr(startPos, endPos - startPos);
                }
                else
                {
                    _generator = line.substr(startPos);
                }
            }
        }
        // Make the result in JSON format.
        makeJsonResult(ErrorCode::SUCCESS);
    }
    else
    {
        makeJsonResult(ErrorCode::COMMAND_ERROR);
    }
}

void ODFValidator::makeJsonResult(ErrorCode errorCode)
{
    bool success = (errorCode == ErrorCode::SUCCESS) ? true : false;
    _jsonResult = "{\n";
    _jsonResult += "    \"success\": ";
    _jsonResult += success ? "true" : "false";
    _jsonResult += ",\n";
    _jsonResult += "    \"errorCode\": ";
    _jsonResult += std::to_string(static_cast<int>(errorCode));
    _jsonResult += ",\n";
    _jsonResult += "    \"errorMessage\": \"";
    _jsonResult += _errorMap[errorCode];
    _jsonResult += "\"";
    // Only add result if success.
    if (success)
    {
        _jsonResult += ",\n";
        _jsonResult += "    \"result\":\n";
        _jsonResult += "    {\n";
        _jsonResult += "        \"validation\": ";
        _jsonResult += _validation ? "true" : "false";
        _jsonResult += ",\n";
        _jsonResult += "        \"generator\": \"";
        _jsonResult += _generator;
        _jsonResult += "\"\n";
        _jsonResult += "    }\n";
        _error.clear();
    }
    else
    {
        _error = _errorMap[errorCode];
    }

    // Add a new line at the end.
    if (_jsonResult.back() != '\n')
    {
        _jsonResult += "\n";
    }

    // End of JSON.
    _jsonResult += "}";

    _returnCode = success ? 0 : 1;
}

// C 接口函數實現
extern "C"
{
    ODFValidator* ODFValidator_new()
    {
        return new ODFValidator();
    }

    void ODFValidator_delete(ODFValidator* validator)
    {
        delete validator;
    }

    const char* ODFValidator_check(ODFValidator* validator, const char* filePath)
    {
        std::string file(filePath);
        validator->check(file);
        return validator->getJsonResult();
    }

    const char* ODFValidator_getResult(ODFValidator* validator)
    {
        return validator->getResult();
    }

    const char* ODFValidator_getJsonResult(ODFValidator* validator)
    {
        return validator->getJsonResult();
    }
}

/* vim:set shiftwidth=4 softtabstop=4 expandtab: */
