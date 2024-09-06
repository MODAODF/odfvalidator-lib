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
#include <regex>
#include <nlohmann/json.hpp>

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

std::string ODFValidator::check(std::string &file)
{
    _file = file;
    executeRealCommand();
    return getJsonResult();
}

void ODFValidator::executeRealCommand()
{
    _result = ""; // Clear the result.
    _results.clear(); // Clear the results.

    std::string javaCmd("java");
    javaCmd.append(" -jar ");
    javaCmd.append(ODFVALIDATOR_JAR_PATH);
    javaCmd.append(" " + _params + " ");
    javaCmd.append("\"" + _file + "\"");
    javaCmd.append(" 2>&1");

#if ENABLE_DEBUG
    // 輸出綠色字體
    std::cout << "\033[32m" << "Command: " << javaCmd << "\033[0m" << std::endl;
#endif

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
            _results.push_back(line);
        }
    }
    else
    {
        _result = "Error executing command: " + javaCmd;
        _returnCode = -999;
    }

#if ENABLE_DEBUG
    std::cout << "Result: " << _result << std::endl;
    std::cout << "\033[32m" << "Return code: " << _returnCode << "\033[0m" << std::endl;
#endif
}

bool ODFValidator::isValid() const
{
    return _result.find("Error: ") == std::string::npos;
}

std::string ODFValidator::getLastEditorTool() const
{
    std::regex generatorRegex(R"(Info: Generator: ((?:OxOffice\/\w+(\.\d+)*)|(\S+\/\d+(\.\d+)*)))");
    std::smatch match;
    if (std::regex_search(_result, match, generatorRegex))
    {
        return match[1].str();
    }
    return "";
}

std::string ODFValidator::getJsonResult() const
{
    nlohmann::json jsonResult;
    jsonResult["isValid"] = isValid();
    jsonResult["lastEditorTool"] = getLastEditorTool();
    return jsonResult.dump();
}

/* vim:set shiftwidth=4 softtabstop=4 expandtab: */

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
        static std::string jsonResult = validator->check(file);
        return jsonResult.c_str();
    }

    const char* ODFValidator_getResult(ODFValidator* validator)
    {
        static std::string staticResult = validator->getResult();
        return staticResult.c_str();
    }
}
