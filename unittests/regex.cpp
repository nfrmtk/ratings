#include <string>
#include <userver/utest/utest.hpp>
#include "../src/lib/validation.hpp"
using namespace std::literals;
struct EmailAnswer {
  std::string email;
  bool ans;
};
struct EmailTest : ::testing::TestWithParam<EmailAnswer> {};

INSTANTIATE_UTEST_SUITE_P(
    good_emails, EmailTest,
    ::testing::Values(EmailAnswer{"vasya@pupkin.ru"s, true},
                      EmailAnswer{"vasya.pupkin@mail.ru"s, true},
                      EmailAnswer{"123@123456.com"s, true}));
INSTANTIATE_UTEST_SUITE_P(
    bad_emails, EmailTest,
    ::testing::Values(EmailAnswer{"va...sya@pupkin.ru"s, false},
                      EmailAnswer{"12@3@yahoo.ru"s, false},
                      EmailAnswer{"#123@123456.com"s, false},
                      EmailAnswer{"almaz03.@yandex.ru"s, false},
                      EmailAnswer{"pass@ya/ru"s, false},
                      EmailAnswer{"pass@ya.c"s, false},
                      EmailAnswer{"pass@ya"s, false},
                      EmailAnswer{"pass@ya#.ru"s, false}));

UTEST_P(EmailTest, check) {
  auto& params = GetParam();
  ASSERT_TRUE(ratings_service::isEmailCorrect(params.email) == params.ans);
}
