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
class ODFValidator
{
public:
    ODFValidator();
    virtual ~ODFValidator() {}
    ODFValidator(std::string& file);

    /// @brief Check if the file complies with ODF specifications.
    /// @param file -The file to check.
    /// @return A JSON string with the results.
    std::string check(std::string& file);

private:
    void executeRealCommand();

private:
    std::string _file; // The file to check.

    std::string _result; // The result of the validation.

}; // class ODFValidator

/* vim:set shiftwidth=4 softtabstop=4 expandtab: */
