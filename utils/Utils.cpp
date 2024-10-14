#include "Utils.hpp"

std::string Utils::toLowerCase(std::string s)
{
    std::string result = s;
    for (size_t i = 0; i < result.length(); ++i) {
        result[i] = std::tolower(static_cast<unsigned char>(result[i]));
    }
    return result;
}

std::vector<std::string> Utils::split(std::string& str, char delimiter)
{
    std::vector<std::string> result;
    int i = 0;
    while (str[i])
    {
        if (str[i] != delimiter)
        {
            std::string tmp = "";
            while (str[i] && str[i] != delimiter)
            {
                tmp += str[i];
                i++;
            }
            result.push_back(tmp);
        }
        else
            i++;
    }
    if (result.empty())
        result.push_back("");
    return result;
}

bool Utils::IsAllDigits(const std::string str)
{
    for (std::string::const_iterator it = str.begin(); it != str.end(); ++it)
    {
        if (!std::isdigit(*it))
            return false;
    }
    return true;
}

bool Utils::IsValidAlphaNum(const std::string str)
{
    const std::string additionalChars = "-_";
    for (std::string::const_iterator it = str.begin(); it != str.end(); ++it)
    {
        if (!std::isalnum(*it) && additionalChars.find(*it) == std::string::npos)
            return false;
    }
    return true;
}

std::string Utils::removeCRLF(std::string str)
{
    std::string newStr;
    for (size_t i = 0; i < str.size(); ++i) {
        if (str[i] == '\r' || str[i] == '\n') {
            continue;
        }
        newStr += str[i];
    }
    return newStr;
}

bool Utils::validInput(std::string input1, std::string input2)
{
    Utils utils;
    std::string ErrorTxt;
    int ErrorCount = 0;
    if (!utils.IsAllDigits(input1))
    {
        ErrorTxt = "Port must be a number\n";
        ErrorCount++;
    }
    if (!utils.IsValidAlphaNum(input2))
    {
        ErrorTxt += "Password must be alphanumeric\n";
        ErrorCount++;
    }
    if (input2.length() > 20)
    {
        ErrorTxt += "Password must have maximum 20 characters\n";
        ErrorCount++;
    }
    int port = std::atoi(input1.c_str());
    if (port < 1024 || port > 49151)
    {
        ErrorTxt += "Port must be between 1024 and 49151\n";
        ErrorCount++;
    }
    if (ErrorCount > 0)
    {
        std::cerr << RED <<  "Error Input ["<<  ErrorCount <<"] :" << RESET << std::endl;
        std::cerr << RED << ErrorTxt << RESET;
        return (false);
    }
    return (true);
}