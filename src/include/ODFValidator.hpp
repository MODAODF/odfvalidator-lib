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

class ODFValidator
{
public:
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
    /// @return Return getJsonResult().
    std::string check(std::string& file);

    /// @brief Get the result of the validation.
    /// @return The result.
    std::string getResult() const { return _result; }

    /// @brief  Get the results of the validation.
    /// @return Vector of results.
    std::vector<std::string> getResults() const { return _results; }

    /// @brief Check if the result contains "Error: ".
    /// @return False if the result contains "Error: ", true otherwise.
    bool isValid() const;

    /// @brief Get the last editor tool from the result.
    /// @return The last editor tool.
    std::string getLastEditorTool() const;

    /// @brief Get the result in JSON format.
    /// @return The result in JSON format contains the values of isValid and lastEditorTool.
    std::string getJsonResult() const;

private:
    void executeRealCommand();

private:
    std::string _params; // The parameters to pass to the validator.
    std::string  _file; // The file to check.
    std::string  _result; // The result of the validation.
    std::vector<std::string> _results; // Vector of results.
    int _returnCode; // The return code of the validator.

}; // class ODFValidator

/* vim:set shiftwidth=4 softtabstop=4 expandtab: */

// C 接口函數
extern "C"
{
    ODFValidator* ODFValidator_new();
    void ODFValidator_delete(ODFValidator* validator);
    const char* ODFValidator_check(ODFValidator* validator, const char* filePath);
    const char* ODFValidator_getResult(ODFValidator* validator);
}
