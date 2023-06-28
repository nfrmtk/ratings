//
// Created by nfrmtk on 01.01.2023.
//

#include "view.hpp"
#include <userver/formats/serialize/common_containers.hpp>
#include <userver/server/handlers/http_handler_base.hpp>
#include <userver/storages/postgres/cluster.hpp>
#include <userver/storages/postgres/component.hpp>
#include "../../models/review.hpp"
namespace ratings_service {
namespace {
namespace pg = userver::storages::postgres;
class GetReviews : public userver::server::handlers::HttpHandlerBase {
 public:
  static constexpr std::string_view kName = "handler-get-reviews";

  GetReviews(const userver::components::ComponentConfig& config,
             const userver::components::ComponentContext& component_context)
      : HttpHandlerBase(config, component_context),
        pg_cluster_(
            component_context
                .FindComponent<userver::components::Postgres>("postgres-db-1")
                .GetCluster()) {}

  std::string HandleRequestThrow(
      const userver::server::http::HttpRequest& request,
      userver::server::request::RequestContext&) const override {
    auto email = request.GetArg("email");
    auto game = request.GetArg("game");
    std::vector<TReview> ans;
    if (email.empty() && game.empty()) {
      ans =
          pg_cluster_
              ->Execute(pg::ClusterHostType::kMaster,
                        "SELECT * FROM ratings_schema.reviews")
              .AsContainer<std::vector<ratings_service::TReview>>(pg::kRowTag);
    }
    if (!email.empty() && !game.empty()) {
      ans = pg_cluster_
                ->Execute(pg::ClusterHostType::kMaster,
                          "SELECT * FROM ratings_schema.reviews "
                          "WHERE email = $1 AND game = $2",
                          email, game)
                .AsContainer<std::vector<TReview>>(pg::kRowTag);
    }
    if (email.empty() && !game.empty()) {
      ans = pg_cluster_
                ->Execute(pg::ClusterHostType::kMaster,
                          "SELECT * FROM ratings_schema.reviews WHERE"
                          "game = $1",
                          game)
                .AsContainer<std::vector<TReview>>(pg::kRowTag);
    }
    if (!email.empty() && game.empty()) {
      ans = pg_cluster_
                ->Execute(pg::ClusterHostType::kMaster,
                          "SELECT * FROM ratings_schema.reviews "
                          "WHERE email = $1",
                          email)
                .AsContainer<std::vector<TReview>>(pg::kRowTag);
    }
    userver::formats::json::ValueBuilder builder{ans};
    return userver::formats::json::ToString(builder.ExtractValue());
  }

 private:
  pg::ClusterPtr pg_cluster_;
};
}  // namespace
void AppendGetReviews(userver::components::ComponentList& components) {
  components.Append<GetReviews>();
}
}  // namespace ratings_service