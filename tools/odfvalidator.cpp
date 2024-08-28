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

#include <ODFValidator.hpp>

int main(int argc, char *argv[])
{
    // Check if there is at least one command-line argument
    if (argc < 2) {
        // Display usage instructions
        std::cout << "Usage: " << argv[0] << " <filename>" << std::endl;
        return 1;
    }

    ODFValidator validator;

    // Read the first command-line argument
    std::string filename = argv[1];

    // Call the check method
    std::string result = validator.check(filename);
    std::cout << "Result:" << std::endl << result << std::endl;

    return 0;
}

/* vim:set shiftwidth=4 softtabstop=4 expandtab: */
