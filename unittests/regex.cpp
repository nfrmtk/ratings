#include "../src/lib/validation.hpp"
#include <string>
#include <userver/utest/utest.hpp>
using namespace std::literals;


struct EmailTest : ::testing::TestWithParam<std::string> {};

INSTANTIATE_TEST_SUITE_P(good_emails, EmailTest,
                         ::testing::Values("vasya@pupkin.ru"s,
                                           "vasya.pupkin@mail.ru"s,
                                           "123@123456.com"s));
UTEST_P(EmailTest, good){
  ASSERT_TRUE(ratings_service::isEmailCorrect(GetParam()));
}
INSTANTIATE_TEST_SUITE_P(bad_emails, EmailTest,
                         ::testing::Values("va...sya@pupkin.ru"s,
                                           "12@3@yahoo.ru"s,
                                           "#123@123456.com"s,
                                           "almaz03.@yandex.ru"s,
                                           "pass@ya/ru"s,
                                           "pass@ya.c"s,
                                           "pass@ya"s,
                                           "pass@ya#.ru"s));
  UTEST_P(EmailTest, bad){
  ASSERT_FALSE(ratings_service::isEmailCorrect(GetParam()));
}

