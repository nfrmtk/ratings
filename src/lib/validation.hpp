#pragma once
#include <string>
namespace ratings_service {
bool isEmailCorrect(std::string_view email);
bool isPasswordStrong(std::string_view password);
}  // namespace ratings_service