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

#ifdef _WIN32
    #define DLL_EXPORT __declspec(dllexport)
#else
    #define DLL_EXPORT
#endif

class ODFValidator
{
public:

    enum class ErrorCode
    {
        SUCCESS = 0, // Success.
        NO_JAVA = 1, // Java is not available.
        JAVA_COMMAND_ERROR = 1, // Java command error.

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
    void check(const std::string& file);

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

    /// @brief Get the validation result.
    /// @return True if the file is a valid ODF file, false otherwise.
    bool isValidation() const { return _validation; }

    /// @brief Get the ODF version.
    /// @return The ODF version.
    std::string getODFVersion() const { return _odfVersion; }

    /// @brief Get the ODF version.
    /// @return The ODF version.
    const char* getODFVersion() { return _odfVersion.c_str(); }

    /// @brief Get the generator.
    /// @return The generator.
    std::string getGenerator() const { return _generator; }

    /// @brief Get the generator.
    /// @return The generator.
    const char* getGenerator() { return _generator.c_str(); }

    /// @brief Get validation error messages. Empty if no error.
    /// @return Vector of validation error messages.
    std::vector<std::string> getErrorMessages() const { return _errorMessages; }

    /// @brief Get error message.
    /// @return The error message. Empty if no error.
    std::string getError() const { return _error; }

    /// @brief check if Java is available.
    /// @return True if Java is available, false otherwise.
    static bool isJavaAvailable() { return _javaAvailable; }

    /// @brief Check if the command is available.
    /// @param command
    /// @return True if the command is available, false otherwise.
    static bool isCommandAvailable(const std::string& command);

    /// @brief Escape a JSON string.
    static std::string escapeJSONString(const std::string& input);

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
    static bool _javaAvailable; // True if Java is available, false otherwise.

    std::string _params; // The parameters to pass to the validator.
    std::string  _file; // The file to check.
    std::string  _result; // The result of the validation.
    std::vector<std::string> _results; // Vector of results.
    int _returnCode; // The return code of the validator.

    std::string _error; // The error message.

    bool _validation; // The validation result.
    std::string _odfVersion; // The ODF version.
    std::string _generator; // The last editor tool.
    std::vector<std::string> _errorMessages; // Vector of validation error messages.

    /// @brief The result in JSON format.
    /// {
    ///     "success": true,
    ///     "errorCode": 0,
    ///     "errorMessage": "Success",
    ///     "result":
    ///     {
    ///         "validation": true,
    ///         "odfVersion": "1.2",
    ///         "generator": "OxOffice/6.0.0"
    ///         "errorMessages":
    ///         [
    ///             "Error: The ODF package is not valid",
    ///             "Error: The ODF package shell contain the content.xml file"
    ///         ]
    ///     }
    /// }
    /// @note If success is false, errorCode and errorMessage will be set. and result will be empty.
    /// @return The result in JSON format.
    std::string _jsonResult; // The result in JSON format.

    // Map of error codes and messages.
    std::map<ErrorCode, std::string> _errorMap =
    {
        {ErrorCode::SUCCESS, "Success"},
        {ErrorCode::NO_JAVA, "Java is not available"},
        {ErrorCode::JAVA_COMMAND_ERROR, "Java command error"},
        {ErrorCode::VALID, "The file is a valid ODF file"},
        {ErrorCode::INVALID, "The file is not a valid ODF file"},
        {ErrorCode::FILE_NOT_FOUND, "File not found"},
        {ErrorCode::FILE_NOT_ODF, "File is not an ODF file"}
    };

    // Map of MS Office version.
    std::map<std::string, std::string> _msOfficeVersionMap =
    {
        {"12.0", "Office 2007"},
        {"14.0", "Office 2010"},
        {"15.0", "Office 2013"},
        {"16.0", "Office 2016/ Office 2019/ Office 365"}
    };

}; // class ODFValidator

// C 接口函數
extern "C"
{
    DLL_EXPORT ODFValidator* ODFValidator_new();
    DLL_EXPORT void ODFValidator_delete(ODFValidator* validator);
    DLL_EXPORT const char* ODFValidator_check(ODFValidator* validator, const char* filePath);
    DLL_EXPORT const char* ODFValidator_getResult(ODFValidator* validator);
    DLL_EXPORT const char* ODFValidator_getJsonResult(ODFValidator* validator);
    DLL_EXPORT const char* ODFValidator_getODFVersion(ODFValidator* validator);
    DLL_EXPORT const char* ODFValidator_getGenerator(ODFValidator* validator);
}

/* vim:set shiftwidth=4 softtabstop=4 expandtab: */