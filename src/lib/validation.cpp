#include "validation.hpp"
#include <boost/regex.hpp>
namespace ratings_service {
// https://stackoverflow.com/questions/56907206/using-regex-for-email-input-validation-c
static const char* email_regex =
    R"([_a-z0-9-]+(\.[_a-z0-9-]+)*@[a-z0-9-]+(\.[a-z0-9-]+)*(\.[a-z]{2,4}))";
template <typename String>
bool isEmailCorrect(std::string_view email) {
  boost::regex checker{email_regex};
  const std::string email_str = std::string{email};
  return boost::regex_match(email_str, checker);
}
template <typename String>
bool isPasswordStrong(std::string_view password) {
  bool has_big_letters = false, has_numbers = false,
       has_regular_letters = false;
  for (auto c : password) {
    if (c >= 'A' && c <= 'Z') has_big_letters = true;
    if (c >= '0' && c <= '9') has_numbers = true;
    if (c >= 'a' && c <= 'z') has_regular_letters = true;
  }
  return has_numbers && has_regular_letters && has_big_letters &&
         password.size() >= 6;
}

}  // namespace ratings_service
