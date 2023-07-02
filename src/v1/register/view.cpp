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
#include "../../lib/validation.hpp"
namespace ratings_service {
namespace {
namespace pg = userver::storages::postgres;
class Register : public userver::server::handlers::HttpHandlerBase {
 public:
  static constexpr std::string_view kName = "handler-register";

  Register(const userver::components::ComponentConfig& config,
           const userver::components::ComponentContext& component_context)
      : HttpHandlerBase(config, component_context),
        pg_cluster_(
            component_context
                .FindComponent<userver::components::Postgres>("postgres-db-1")
                .GetCluster()) {}

  std::string HandleRequestThrow(
      const userver::server::http::HttpRequest& request,
      userver::server::request::RequestContext&) const override {
    if (!request.HasFormDataArg("password") ||
        !ratings_service::isPasswordStrong(
            request.GetFormDataArg("password").value) ||
        !request.HasFormDataArg("email") ||
        !ratings_service::isEmailCorrect(
            request.GetFormDataArg("email").value)) {
      request.GetHttpResponse().SetStatus(
          userver::server::http::HttpStatus::kBadRequest);
      return {};
    }
    auto email = request.GetFormDataArg("email").value;
    auto passwd_hash =
        userver::crypto::hash::Sha256(request.GetFormDataArg("password").value);
    auto username = request.GetFormDataArg("username").value;
    auto result =
        pg_cluster_->Execute(pg::ClusterHostType::kMaster,
                             "INSERT INTO ratings_schema.users "
                             "VALUES ($1, $2, $3) ON CONFLICT DO NOTHING;",
                             email, username, passwd_hash);
    if (result.RowsAffected() == 0) {
      request.GetHttpResponse().SetStatus(
          userver::server::http::HttpStatus::kConflict);
    }
    return {};
  }

 private:
  pg::ClusterPtr pg_cluster_;
};
}  // namespace
void AppendRegister(userver::components::ComponentList& components) {
  components.Append<Register>();
}
}  // namespace ratings_service