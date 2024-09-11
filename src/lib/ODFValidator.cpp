/* -*- Mode: C++; tab-width: 4; indent-tabs-mode: nil; c-basic-offset: 4; fill-column: 100 -*- */
/*
 * SPDX-License-Identifier: MPL-2.0
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#if !CMAKE
#include "config.h"
#endif

#if WINDOWS
#define popen _popen
#define pclose _pclose
#define JAVA_CMD "java.exe"
#define PATH_SPLITTER ';' // Windows uses semicolon.
#else
#include <unistd.h>
#define JAVA_CMD "java"
#define PATH_SPLITTER ':' // Unix uses colon.
#endif

#include <stdio.h>
#include <string>
#include <iostream>
#include <sstream>
#include <filesystem>
#include <cstdlib> // for system()

#include <ODFValidator.hpp>

bool ODFValidator::_javaAvailable = ODFValidator::isCommandAvailable(JAVA_CMD);

ODFValidator::ODFValidator()
{
    _params = "-v -e"; // Default parameters.
}

bool ODFValidator::isCommandAvailable(const std::string& command)
{
    std::vector<std::string> paths;
    const char* pathEnv = std::getenv("PATH");
    if (pathEnv != nullptr) {
        std::string pathStr(pathEnv);
        size_t pos = 0;
        std::string token;
        while ((pos = pathStr.find(PATH_SPLITTER)) != std::string::npos) {
            token = pathStr.substr(0, pos);
            paths.push_back(token);
            pathStr.erase(0, pos + 1);
        }
        paths.push_back(pathStr);
    }

    for (const auto& path : paths) {
        std::filesystem::path fullPath = std::filesystem::path(path) / command;
        if (std::filesystem::exists(fullPath))
        {
            return true;
        }
    }
    return false;
}

std::string ODFValidator::escapeJSONString(const std::string& input)
{
    std::string output;
    output.reserve(input.size() + 2);
    for (char c : input)
    {
        switch (c)
        {
            case '"':
                output += "\\\"";
                break;
            case '\\':
                output += "\\\\";
                break;
            case '\b':
                output += "\\b";
                break;
            case '\f':
                output += "\\f";
                break;
            case '\n':
                output += "\\n";
                break;
            case '\r':
                output += "\\r";
                break;
            case '\t':
                output += "\\t";
                break;
            default:
                output += c;
                break;
        }
    }
    return output;
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

void ODFValidator::check(const std::string &file)
{
    _file = file;
    // 用 filesystem::exists() 檢查檔案是否存在
    if (!std::filesystem::exists(_file))
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
    _odfVersion = "unknown"; // Set the ODF version to unknown.
    _generator.clear(); // Clear the generator.

    _validation = true; // Set the validation result to true.
    _errorMessages.clear(); // Clear the error messages.

    if (!isJavaAvailable())
    {
        makeJsonResult(ErrorCode::NO_JAVA);
        return;
    }

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

            // Parse " is invalid for the ODF XML Schema document"
            if (line.find(" is invalid for the ODF XML Schema document") != std::string::npos)
            {
                makeJsonResult(ErrorCode::FILE_NOT_ODF);
                return;
            }

            // Parse "Error: " to get the validation result.
            const std::size_t foundError = line.find("Error: ");
            if (foundError != std::string::npos)
            {
                _validation = false;
                _errorMessages.push_back(line);
            }

            // Parse "ODF version of root document: " to get the ODF version.
            const std::size_t foundODFVersion = line.find("ODF version of root document: ");
            if (foundODFVersion != std::string::npos)
            {
                std::size_t startPos = foundODFVersion + 30;
                // find first space after "ODF version of root document: "
                std::size_t endPos = line.find(" ", startPos);
                if (endPos != std::string::npos)
                {
                    _odfVersion = line.substr(startPos, endPos - startPos);
                }
                else
                {
                    _odfVersion = line.substr(startPos);
                }
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
        makeJsonResult(ErrorCode::JAVA_COMMAND_ERROR);
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
    _jsonResult += escapeJSONString(_errorMap[errorCode]);
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
        _jsonResult += "        \"odfVersion\": \"";
        _jsonResult += escapeJSONString(_odfVersion);
        _jsonResult += "\",\n";
        _jsonResult += "        \"generator\": \"";
        _jsonResult += escapeJSONString(_generator);
        _jsonResult += "\"\n";
        if (!_errorMessages.empty())
        {
            _jsonResult += ",\n";
            _jsonResult += "        \"errorMessages\": [\n";
            for (const auto &error : _errorMessages)
            {
                _jsonResult += "            \"";
                _jsonResult += escapeJSONString(error);
                _jsonResult += "\",\n";
            }
            _jsonResult.pop_back(); // Remove the last comma.
            _jsonResult.pop_back(); // Remove the last newline.
            _jsonResult += "\n";
            _jsonResult += "        ]\n";
        }
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
