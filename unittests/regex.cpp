#include "../src/lib/validation.hpp"
#include <string>
#include <userver/utest/utest.hpp>
using namespace std::literals;


struct CorrectEmailTest : ::testing::TestWithParam<std::string> {};

INSTANTIATE_UTEST_SUITE_P(pass_true, CorrectEmailTest,
                         ::testing::Values("vasya@pupkin.ru"s,
                                           "vasya.pupkin@mail.ru"s,
                                           "123@123456.com"s));

UTEST_P(CorrectEmailTest, pass_true){
  ASSERT_TRUE(ratings_service::isEmailCorrect(GetParam()));
}
