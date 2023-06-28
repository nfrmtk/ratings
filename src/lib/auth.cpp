//
// Created by nfrmtk on 28.06.2023.
//

#include "auth.hpp"
namespace ratings_service {
std::optional<TSessionInfo> GetSessionInfo(
    userver::storages::postgres::ClusterPtr ptr,
    const userver::server::http::HttpRequest& request) {
  if (!request.HasHeader(USER_TICKET_HEADER_NAME)) {
    return std::nullopt;
  }
  const auto& header = request.GetHeader(USER_TICKET_HEADER_NAME);
  auto res = ptr->Execute(userver::storages::postgres::ClusterHostType::kMaster,
                          "SELECT * FROM ratings_schema.auth_sessions "
                          "where id = $1;",
                          header);
  if (res.IsEmpty())
    return std::nullopt;
  else
    return std::make_optional(
        res.AsSingleRow<TSessionInfo>(userver::storages::postgres::kRowTag));
}
}  // namespace ratings_service
