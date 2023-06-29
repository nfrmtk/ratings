//
// Created by nfrmtk on 01.01.2023.
//

#include "view.hpp"
#include <userver/server/handlers/http_handler_base.hpp>
#include <userver/server/http/http_request.hpp>
#include <userver/server/request/request_context.hpp>
#include <userver/storages/postgres/cluster.hpp>
#include <userver/storages/postgres/component.hpp>
#include "../../lib/auth.hpp"

namespace ratings_service {
namespace {
namespace pg = userver::storages::postgres;
class UpdateReview : public userver::server::handlers::HttpHandlerBase {
 public:
  static constexpr std::string_view kName = "handler-update-review";

  UpdateReview(const userver::components::ComponentConfig& config,
               const userver::components::ComponentContext& component_context)
      : HttpHandlerBase(config, component_context),
        pg_cluster_(
            component_context
                .FindComponent<userver::components::Postgres>("postgres-db-1")
                .GetCluster()) {}

  std::string HandleRequestThrow(
      const userver::server::http::HttpRequest& request,
      userver::server::request::RequestContext&) const override {
    auto& response = request.GetHttpResponse();
    auto info = GetSessionInfo(pg_cluster_, request);
    if (!info.has_value()) {
      response.SetStatus(userver::server::http::HttpStatus::kUnauthorized);
      return {};
    }
    auto body = userver::formats::json::FromString(request.RequestBody());
    if (!body.HasMember("game")) {
      response.SetStatus(userver::server::http::HttpStatus::kBadRequest);
      return {};
    }
    auto game = body["game"].As<std::string>();
    auto email = std::get<1>(*info);
    bool hasNewRating = body.HasMember("rating");
    bool hasNewText = body.HasMember("text");
    std::size_t rowsAffected = 0;
    if (hasNewRating && hasNewText) {
      auto rating = body["rating"].As<int32_t>();
      auto text = body["text"].As<std::string>();
      rowsAffected = pg_cluster_
                         ->Execute(pg::ClusterHostType::kMaster,
                                   "UPDATE ratings_schema.reviews "
                                   "SET review = $4, rating = $3 "
                                   "where email = $1 AND game = $2",
                                   email, game, rating, text)
                         .RowsAffected();
    }
    if (!hasNewText && hasNewRating) {
      auto rating = body["rating"].As<int32_t>();
      rowsAffected = pg_cluster_
                         ->Execute(pg::ClusterHostType::kMaster,
                                   "UPDATE ratings_schema.reviews "
                                   "SET rating = $3 "
                                   "WHERE email = $1 AND game = $2",
                                   email, game, rating)
                         .RowsAffected();
    }
    if (!hasNewRating && hasNewText) {
      auto text = body["text"].As<std::string>();
      rowsAffected = pg_cluster_
                         ->Execute(pg::ClusterHostType::kMaster,
                                   "UPDATE ratings_schema.reviews "
                                   "SET review = $3 "
                                   "where email = $1 AND game = $2",
                                   email, game, text)
                         .RowsAffected();
    }
    if (rowsAffected == 0) {
      response.SetStatus(userver::server::http::HttpStatus::kBadRequest);
      return {};
    }
    return {};
  }

 private:
  pg::ClusterPtr pg_cluster_;
};
}  // namespace
void AppendUpdateReview(userver::components::ComponentList& components) {
  components.Append<UpdateReview>();
}
}  // namespace ratings_service