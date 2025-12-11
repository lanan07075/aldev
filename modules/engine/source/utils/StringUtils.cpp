//
// Created by dawn on 2025/10/23.
//

#include "StringUtils.hpp"

bool StringUtils::starts_with(std::string str, std::string prefix)
{
    return (str.compare(0, prefix.size(), prefix) == 0);
}