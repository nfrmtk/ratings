#include "../src/lib/validation.hpp"
#include <string>
#include <userver/utest/utest.hpp>
using namespace std::literals;


struct CorrectEmailTest : ::testing::TestWithParam<std::string> {};

INSTANTIATE_TEST_SUITE_P(results_true, CorrectEmailTest,
                         ::testing::Values("vasya@pupkin.ru"s,
                                           "vasya.pupkin@mail.ru"s,
                                           "123@123456.com"s,
                                           "almaz03@yandex.ru"s));

UTEST_P(CorrectEmailTest, pass_true){
  ASSERT_TRUE(ratings_service::isEmailCorrect(GetParam()));
}

struct IncorrectEmailTest : ::testing::TestWithParam<std::string> {};

INSTANTIATE_TEST_SUITE_P(results_false, IncorrectEmailTest,
                         ::testing::Values("va...sya@pupkin.ru"s,
                                           "12@3@yahoo.ru"s,
                                           "#123@123456.com"s,
                                           "almaz03.@yandex.ru"s,
                                           "pass@ya/ru"s,
                                           "pass@ya.c"s,
                                           "pass@ya"s,
                                           "pass@ya#.ru"s));

UTEST_P(IncorrectEmailTest, pass_false){
  ASSERT_FALSE(ratings_service::isEmailCorrect(GetParam()));
}