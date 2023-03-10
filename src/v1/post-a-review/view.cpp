//
// Created by nfrmtk on 01.01.2023.
//

#include "view.hpp"
#include <userver/server/handlers/http_handler_base.hpp>
#include <userver/server/http/http_request.hpp>
#include <userver/server/request/request_context.hpp>
#include <userver/storages/postgres/cluster.hpp>
#include <userver/storages/postgres/component.hpp>

namespace ratings_service {
namespace {
namespace pg = userver::storages::postgres;
class PostReview : public userver::server::handlers::HttpHandlerBase {
 public:
  static constexpr std::string_view kName = "handler-post-review";

  PostReview(const userver::components::ComponentConfig& config,
             const userver::components::ComponentContext& component_context)
      : HttpHandlerBase(config, component_context),
        pg_cluster_(
            component_context
                .FindComponent<userver::components::Postgres>("postgres-db-1")
                .GetCluster()) {}

  std::string HandleRequestThrow(
      const userver::server::http::HttpRequest& request,
      userver::server::request::RequestContext&) const override {
    auto body = userver::formats::json::FromString(request.RequestBody());
    auto rating = body["rating"].As<int32_t>();
    auto text = body["text"].As<std::string>();
    auto game = body["game"].As<std::string>();
    auto username = body["username"].As<std::string>();
    auto result = pg_cluster_
                      ->Execute(pg::ClusterHostType::kMaster,
                                "INSERT INTO ratings_schema.reviews(username, "
                                "game, rating, review) VALUES($1, $2, $3, $4) "
                                "ON CONFLICT DO NOTHING "
                                "RETURNING reviews.created_at",
                                username, game, rating, text);
    pg::TimePointTz timing;
    if (!result.IsEmpty()) timing = result.AsSingleRow<pg::TimePointTz>(pg::kFieldTag);
    return userver::utils::datetime::LocalTimezoneTimestring(
        timing.GetUnderlying());
  }

 private:
  pg::ClusterPtr pg_cluster_;
};
}  // namespace
void AppendPostReview(userver::components::ComponentList& components) {
  components.Append<PostReview>();
}
}  // namespace ratings_service