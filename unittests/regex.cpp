#include "../src/lib/validation.hpp"
#include <string>
#include <userver/utest/utest.hpp>
using namespace std::literals;


struct CorrectEmailTest : ::testing::TestWithParam<std::string> {};


UTEST_P(CorrectEmailTest, pass_true){
  ASSERT_TRUE(ratings_service::isEmailCorrect(GetParam()));
}
INSTANTIATE_TEST_SUITE_P(results_true, CorrectEmailTest,
                         ::testing::Values("vasya@pupkin.ru"s,
                                           "vasya.pupkin@mail.ru"s,
                                           "123@123456.com"s,
                                           "almaz03@yandex.ru"s));

struct IncorrectEmailTest : ::testing::TestWithParam<std::string> {};


UTEST_P(IncorrectEmailTest, pass_false){
  ASSERT_FALSE(ratings_service::isEmailCorrect(GetParam()));
}

INSTANTIATE_TEST_SUITE_P(results_false, IncorrectEmailTest,
                         ::testing::Values("va...sya@pupkin.ru"s,
                                           "12@3@yahoo.ru"s,
                                           "#123@123456.com"s,
                                           "almaz03.@yandex.ru"s,
                                           "pass@ya/ru"s,
                                           "pass@ya.c"s,
                                           "pass@ya"s,
                                           "pass@ya#.ru"s));

int main(int argc, char* argv[]){
  testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}