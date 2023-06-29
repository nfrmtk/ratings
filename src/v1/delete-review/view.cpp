//
// Created by nfrmtk on 01.01.2023.
//

#include "view.hpp"
#include <stdexcept>
#include <userver/server/handlers/http_handler_base.hpp>
#include <userver/server/http/http_request.hpp>
#include <userver/server/request/request_context.hpp>
#include <userver/storages/postgres/cluster.hpp>
#include <userver/storages/postgres/component.hpp>
#include "../../lib/auth.hpp"

namespace ratings_service {
namespace {
namespace pg = userver::storages::postgres;
class Delete : public userver::server::handlers::HttpHandlerBase {
 public:
  static constexpr std::string_view kName = "handler-delete-review";

  Delete(const userver::components::ComponentConfig& config,
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
      response.SetStatus(
          userver::server::http::HttpStatus::kUnauthorized);
      return {};
    }
    auto game = request.GetArg("game");
    auto email = std::get<1>(*info);
    auto result = pg_cluster_->Execute(pg::ClusterHostType::kMaster,
                                       "DELETE FROM ratings_schema.reviews "
                                       "WHERE email = $1 AND game = $2",
                                       email, game);
    switch (result.RowsAffected()) {
      case 0: {
        response.SetStatus(userver::server::http::HttpStatus::kBadRequest);
        break;
      }
      case 1: {
        break;
      }
      default: {
        throw std::runtime_error(
            "Number of deleted lines must not be greater than 1. "
            "Possible cause: duplicaated rows in database.");
      }
    }
    return {};
  }

 private:
  pg::ClusterPtr pg_cluster_;
};
}  // namespace
void AppendDeleteReview(userver::components::ComponentList& components) {
  components.Append<Delete>();
}
}  // namespace ratings_service