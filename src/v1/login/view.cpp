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
namespace ratings_service {
namespace {
namespace pg = userver::storages::postgres;
class Login : public userver::server::handlers::HttpHandlerBase {
 public:
  static constexpr std::string_view kName = "handler-login";

  Login(const userver::components::ComponentConfig& config,
        const userver::components::ComponentContext& component_context)
      : HttpHandlerBase(config, component_context),
        pg_cluster_(
            component_context
                .FindComponent<userver::components::Postgres>("postgres-db-1")
                .GetCluster()) {}

  std::string HandleRequestThrow(
      const userver::server::http::HttpRequest& request,
      userver::server::request::RequestContext&) const override {
    auto email = request.GetFormDataArg("email").value;
    auto passwd_hash =
        userver::crypto::hash::Sha256(request.GetFormDataArg("password").value);
    auto result = pg_cluster_->Execute(pg::ClusterHostType::kMaster,
                                       "SELECT users.password_hash = $2 "
                                       "FROM ratings_schema.users "
                                       "WHERE email = $1",
                                       email, passwd_hash);
    auto& response = request.GetHttpResponse();
    if (result.IsEmpty()) {
      response.SetStatus(userver::server::http::HttpStatus::kBadRequest);
      return {};
    }
    bool isPasswordCorrect =
        result.AsSingleRow<bool>(userver::storages::postgres::kFieldTag);
    if (!isPasswordCorrect) {
      response.SetStatus(userver::server::http::HttpStatus::kUnauthorized);
      return {};
    }
    auto uuid = pg_cluster_
                    ->Execute(pg::ClusterHostType::kMaster,
                              "INSERT INTO ratings_schema.auth_sessions "
                              "VALUES (DEFAULT, $1, DEFAULT) ON CONFLICT"
                              "RETURNING id;",
                              email)
                    .AsSingleRow<std::string>(pg::kFieldTag);
    userver::formats::json::ValueBuilder builder;
    builder["ticket"] = uuid;
    return userver::formats::json::ToString(builder.ExtractValue());
  }

 private:
  pg::ClusterPtr pg_cluster_;
};
}  // namespace
void AppendLogin(userver::components::ComponentList& components) {
  components.Append<Login>();
}
}  // namespace ratings_service