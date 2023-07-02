#include "validation.hpp"
#include <regex>
namespace ratings_service {
// https://stackoverflow.com/questions/56907206/using-regex-for-email-input-validation-c
static const char* email_regex =
    R"([_a-z0-9-]+(\.[_a-z0-9-]+)*@[a-z0-9-]+(\.[a-z0-9-]+)*(\.[a-z]{2,4}))";
bool isEmailCorrect(const std::string& email) {
  std::regex checker{email_regex};
  return std::regex_match(email, checker);
}
bool isPasswordStrong(const std::string& password) {}

}  // namespace ratings_service
