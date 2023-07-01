#pragma once
#include <string>
namespace ratings_service{
bool isEmailCorrect(const std::string& email);
bool isPasswordStrong(const std::string& password);
}