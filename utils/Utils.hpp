#ifndef UTILS_HPP
#define UTILS_HPP

#include <iostream>
#include <string>
#include <vector>
#include <sstream>
#include <ctype.h>
#include "../server/Server.hpp"

class Utils
{
    public:
        std::vector<std::string> split(std::string& str, char delimiter);
        std::string removeCRLF(std::string str);
        std::string toLowerCase(std::string s);
        bool IsAllDigits(const std::string str);
        bool IsValidAlphaNum(const std::string str);
        bool validInput(std::string input1, std::string input2);
};

#endif