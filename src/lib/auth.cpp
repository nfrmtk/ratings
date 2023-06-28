//
// Created by nfrmtk on 28.06.2023.
//

#include "auth.hpp"
#include <chrono>
using namespace std::chrono_literals;
namespace ratings_service {
std::optional<TSessionInfo> GetSessionInfo(
    userver::storages::postgres::ClusterPtr ptr,
    const userver::server::http::HttpRequest& request) {
  // TODO: properly handle more than 1 device from 1 user
  if (!request.HasHeader(USER_TICKET_HEADER_NAME)) {
    return std::nullopt;
  }
  const auto& header = request.GetHeader(USER_TICKET_HEADER_NAME);
  auto res = ptr->Execute(
      userver::storages::postgres::ClusterHostType::kMaster,
      "SELECT * FROM ratings_schema.auth_sessions "
      "where id = $1 AND created_at = (SELECT "
      "MAX(created_at) FROM ratings_schema.auth_sessions WHERE id = $1)",
      header);
  if (res.IsEmpty()) {
    return std::nullopt;
  }

  auto time = std::get<2>(res.AsSingleRow<TSessionInfo>(
                              userver::storages::postgres::kFieldTag))
                  .GetUnderlying();
  if (std::chrono::system_clock::now() - time > 10min) {
    if (rand() % 10 == 0) // haha
    {
      ptr->Execute(
          userver::storages::postgres::ClusterHostType::kMaster,
          "DELETE FROM ratings_schema.auth_sessions WHERE CURRENT_TIMESTAMP > "
          "auth_sessions.created_at + interval '10 minutes';");
    }
    return std::nullopt;
  }
  return std::make_optional(
      res.AsSingleRow<TSessionInfo>(userver::storages::postgres::kRowTag));
}
}  // namespace ratings_service
