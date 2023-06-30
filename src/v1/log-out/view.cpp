//
// Created by nfrmtk on 01.01.2023.
//

#include "view.hpp"
#include <userver/crypto/hash.hpp>
#include <userver/server/handlers/http_handler_base.hpp>
#include <userver/server/http/http_request.hpp>
#include <userver/server/request/request_context.hpp>
#include <userver/storages/postgres/cluster.hpp>
#include <userver/storages/postgres/component.hpp>
#include <userver/utils/assert.hpp>
#include "../../lib/auth.hpp"

namespace ratings_service {
namespace {
namespace pg = userver::storages::postgres;
class LogOut : public userver::server::handlers::HttpHandlerBase {
 public:
  static constexpr std::string_view kName = "handler-log-out";

  LogOut(const userver::components::ComponentConfig& config,
         const userver::components::ComponentContext& component_context)
      : HttpHandlerBase(config, component_context),
        pg_cluster_(
            component_context
                .FindComponent<userver::components::Postgres>("postgres-db-1")
                .GetCluster()) {}

  std::string HandleRequestThrow(
      const userver::server::http::HttpRequest& request,
      userver::server::request::RequestContext&) const override {
    auto info = GetSessionInfo(pg_cluster_, request);
    if (!info.has_value()) {
      request.GetHttpResponse().SetStatus(
          userver::server::http::HttpStatus::kUnauthorized);
      return {};
    }
    auto result = pg_cluster_->Execute(
        pg::ClusterHostType::kMaster,
        "DELETE FROM ratings_schema.auth_sessions WHERE email = $1;",
        std::get<1>(*info));
    UASSERT_MSG(result.RowsAffected() != 0,
                "At least 1 session must be deleted!");
    return {};
  }

 private:
  pg::ClusterPtr pg_cluster_;
};
}  // namespace
void AppendLogOut(userver::components::ComponentList& components) {
  components.Append<LogOut>();
}
}  // namespace ratings_service