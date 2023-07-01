#include "../src/lib/validation.hpp"
#include <string>
#include <userver/utest/utest.hpp>
using namespace std::literals;


struct EmailTest : ::testing::TestWithParam<std::string> {};

INSTANTIATE_TEST_SUITE_P(results_true, EmailTest,
                         ::testing::Values("vasya@pupkin.ru"s,
                                           "vasya.pupkin@mail.ru"s,
                                           "123@123456.com"s,
                                           "almaz03@yandex.ru"s));

UTEST_P(EmailTest, correct_emails){
  ASSERT_TRUE(ratings_service::isEmailCorrect(GetParam()));
}
INSTANTIATE_TEST_SUITE_P(results_false, EmailTest,
                         ::testing::Values("va...sya@pupkin.ru"s,
                                           "12@3@yahoo.ru"s,
                                           "#123@123456.com"s,
                                           "almaz03.@yandex.ru"s,
                                           "pass@ya/ru"s,
                                           "pass@ya.c"s,
                                           "pass@ya"s,
                                           "pass@ya#.ru"s));

UTEST_P(EmailTest, incorrect_emails){
  ASSERT_FALSE(ratings_service::isEmailCorrect(GetParam()));
}
