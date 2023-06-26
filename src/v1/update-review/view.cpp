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
    auto body = userver::formats::json::FromString(request.RequestBody());
    auto rating = body["rating"].As<int32_t>();
    auto text = body["text"].As<std::string>();
    auto game = body["game"].As<std::string>();
    auto username = body["username"].As<std::string>();
    auto result = pg_cluster_
                      ->Execute(pg::ClusterHostType::kMaster,
                                "UPDATE ratings_schema.reviews "
                                "SET review = $4, rating = $3 "
                                "where username = $1 AND game = $2",
                                username, game, rating, text);
    if (result.RowsAffected() == 0) {
      auto& response = request.GetHttpResponse();
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