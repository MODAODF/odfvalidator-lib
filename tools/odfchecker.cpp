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
#include <vector>
#include <set>
#include <map>
#include <fstream>

#include <ODFValidator.hpp>

int main(int argc, char *argv[])
{
    // 至少需要一個參數
    if (argc < 2)
    {
        std::cout << "Usage: " << argv[0] << " <file> [file2 file3 ...]" << std::endl;
        std::cout << "Note: File names can use wildcard characters such as '*' and '?'." << std::endl;
        return 1;
    }

    // 用來存放錯誤原因（不重複）
    std::set<std::string> errorReasons;

    // 用來存放檔案和錯誤原因
    std::map<std::string, std::vector<std::string>> errorTable;

    // Read the external parameters
    for (int i = 1; i < argc; i++)
    {
        ODFValidator validator; // Create an instance of the ODFValidator class.
        const std::string defaultParams = "-v -e -w"; // Default validation parameters.
        const std::string filename = argv[i];

        std::cout << "Checking : " << filename << " ... ";
        validator.setParams(defaultParams);
        validator.check(filename);
        int returnCode = validator.getReturnCode();
        if (returnCode != 0)
        {
            // 輸出亮紅色 Error: 錯誤信息
            std::cout << "\033[1;31merror: " << validator.getError() << "\033[0m" << std::endl;
        }
        else
        {
            // 輸出亮綠色
            std::cout << "\033[1;32mdone" << "\033[0m";
            // Get results
            const std::vector<std::string> results = validator.getResults();
            bool hasError = false;
            // 找出有 "Error: " 的行
            for (const auto &result : results)
            {
                const std::size_t foundError = result.find("Error: ");
                if (foundError != std::string::npos)
                {
                    hasError = true;
                    std::string firstPart = result.substr(0, foundError);
                    std::string reason = result.substr(foundError);
                    // 去掉 firstPart 前後空白
                    firstPart.erase(firstPart.find_last_not_of(" \n\r\t")+1);
                    // 去掉 reason 前後空白
                    reason.erase(0, reason.find_first_not_of(" \n\r\t"));

                    // 如果 reason 不在 errorReasons 裡面，就加進去
                    if (errorReasons.find(reason) == errorReasons.end())
                    {
                        errorReasons.insert(reason);
                    }

                    // 如果 filename 不在 errorTable 裡面，就加進去
                    if (errorTable.find(filename) == errorTable.end())
                    {
                        errorTable[filename] = std::vector<std::string>();
                    }
                    errorTable[filename].push_back(result);

                }
            }
            if (!hasError)
            {
                std::cout << std::endl;
            }
            else
            {
                std::cout << "(but error)" << std::endl;
            }
        }
    }

    // dump errorReasons
    if (errorReasons.size() > 0)
    {
        std::cout << "Error reasons(not duplicate):" << std::endl;
        for (const auto &reason : errorReasons)
        {
            std::cout << reason << std::endl;
        }
    }

    // 有檔案檢測出錯誤
    if (errorTable.size() > 0)
    {
        std::string saveToFile;
        std::cout << "Save to file? (Enter file name or press Enter to show on screen): ";
        std::getline(std::cin, saveToFile);
        // remove head and tail space
        saveToFile.erase(saveToFile.find_last_not_of(" \n\r\t")+1);

        if (saveToFile.length() > 0)
        {
            std::ofstream ofs(saveToFile);
            if (ofs.is_open())
            {
                for (const auto &error : errorTable)
                {
                    //ofs << "File: " << error.first << std::endl;
                    for (const auto &reason : error.second)
                    {
                        ofs << reason << std::endl;
                    }
                }
                ofs.close();
                std::cout << "Success save to file: " << saveToFile << std::endl;
            }
            else
            {
                std::cout << "Can not save to file: " << saveToFile << std::endl;
            }
        }
        else
        {
            for (const auto &error : errorTable)
            {
                std::cout << "File: " << error.first << std::endl;
                for (const auto &reason : error.second)
                {
                    std::cout << reason << std::endl;
                }
                std::cout << "----------------------------------------" << std::endl;
            }
        }
    }

    return 0;
}

/* vim:set shiftwidth=4 softtabstop=4 expandtab: */
