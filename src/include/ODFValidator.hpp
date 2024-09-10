/* -*- Mode: C++; tab-width: 4; indent-tabs-mode: nil; c-basic-offset: 4; fill-column: 100 -*- */
/*
 * SPDX-License-Identifier: MPL-2.0
 *
 * This Source Code Form is subject to the terms of the Mozilla Public
 * License, v. 2.0. If a copy of the MPL was not distributed with this
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.
 */

#pragma once

#include <string>
#include <vector>
#include <map>

class ODFValidator
{
public:

    enum class ErrorCode
    {
        SUCCESS = 0, // Success.
        COMMAND_ERROR = 1, // Command error.

        VALID = 100, // The file is a valid ODF file.
        INVALID = 101, // The file is not a valid ODF file.

        FILE_NOT_FOUND = 404, // File not found.
        FILE_NOT_ODF = 405, // File is not an ODF file.
    };

    ODFValidator();
    virtual ~ODFValidator() {}

    /// @brief Show detailed help.
    void showHelp();

    /// @brief Set the parameters to pass to the validator.
    /// @param params
    void setParams(const std::string& params) { _params = params; }

    /// @brief Get the parameters to pass to the validator.
    /// @return The parameters.
    std::string getParams() const { return _params; }

    /// @brief Check if the file complies with ODF specifications.
    /// @param file -The file to check.
    void check(std::string& file);

    /// @brief Get the result of the validation.
    /// @return The result.
    std::string getResult() const { return _result; }

    /// @brief Get the results of the validation.
    const char* getResult() { return _result.c_str(); }

    /// @brief  Get the results of the validation.
    /// @return Vector of results.
    std::vector<std::string> getResults() const { return _results; }

    /// @brief Get the result of the validation.
    /// @return The result.
    std::string getJsonResult() const { return _jsonResult; }

    /// @brief Get the results of the validation.
    const char* getJsonResult() { return _jsonResult.c_str(); }

    /// @brief Get the return code of the validator.
    /// @return The return code. 0 if success, antoher value if error.
    int getReturnCode() const { return _returnCode; }

    /// @brief Get error message.
    /// @return The error message. Empty if no error.
    std::string getError() const { return _error; }

private:
    /// @brief Check if the file is a ZIP file.
    /// @param file
    /// @return True if the file is a ZIP file, false otherwise.
    bool isZipFile(const std::string& file);

    /// @brief Check if the file is an ODF file.
    /// @param file
    /// @return True if the file is an ODF file, false otherwise.
    bool isODFFile(const std::string& file);

    /// @brief Execute the real command.
    void executeRealCommand();

    /// @brief Make the result in JSON format.
    void makeJsonResult(ErrorCode errorCode);

private:
    std::string _params; // The parameters to pass to the validator.
    std::string  _file; // The file to check.
    std::string  _result; // The result of the validation.
    std::vector<std::string> _results; // Vector of results.
    int _returnCode; // The return code of the validator.

    std::string _error; // The error message.

    bool _validation; // The validation result.
    std::string _generator; // The last editor tool.

    /// @brief The result in JSON format.
    /// {
    ///     "success": true,
    ///     "errorCode": 0,
    ///     "errorMessage": "Success",
    ///      "result":
    ///     {
    ///         "validation": true,
    ///         "generator": "OxOffice/6.0.0"
    ///     }
    /// }
    /// @note If success is false, errorCode and errorMessage will be set. and result will be empty.
    /// @return The result in JSON format.
    std::string _jsonResult; // The result in JSON format.

    // Map of error codes and messages.
    std::map<ErrorCode, std::string> _errorMap =
    {
        {ErrorCode::SUCCESS, "Success"},
        {ErrorCode::COMMAND_ERROR, "Command error"},
        {ErrorCode::VALID, "The file is a valid ODF file"},
        {ErrorCode::INVALID, "The file is not a valid ODF file"},
        {ErrorCode::FILE_NOT_FOUND, "File not found"},
        {ErrorCode::FILE_NOT_ODF, "File is not an ODF file"}
    };

}; // class ODFValidator

// C 接口函數
extern "C"
{
    ODFValidator* ODFValidator_new();
    void ODFValidator_delete(ODFValidator* validator);
    const char* ODFValidator_check(ODFValidator* validator, const char* filePath);
    const char* ODFValidator_getResult(ODFValidator* validator);
    const char* ODFValidator_getJsonResult(ODFValidator* validator);
}

/* vim:set shiftwidth=4 softtabstop=4 expandtab: */