//
// Created by nfrmtk on 02.01.2023.
//
#include <string>
#include <userver/formats/json/value.hpp>
#include <userver/formats/json/value_builder.hpp>

namespace ratings_service {
struct TReview {
  std::string username;
  std::string game;
  std::int32_t rating;
  std::string text;
};

userver::formats::json::Value Serialize(
    const TReview& data,
    userver::formats::serialize::To<userver::formats::json::Value>);

}  // namespace ratings_service