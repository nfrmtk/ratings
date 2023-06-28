//
// Created by nfrmtk on 02.01.2023.
//

#include "./review.hpp"
namespace ratings_service {
userver::formats::json::Value Serialize(
    const TReview& data,
    userver::formats::serialize::To<userver::formats::json::Value>) {
  userver::formats::json::ValueBuilder builder;
  builder["email"] = data.email;
  builder["game"] = data.game;
  builder["rating"] = data.rating;
  builder["text"] = data.text;
  builder["created_at"] = data.created_at;
  return builder.ExtractValue();
}

}  // namespace ratings_service