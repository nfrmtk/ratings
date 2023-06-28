#pragma once
#include <optional>
#include <string>
#include <tuple>
#include <userver/server/http/http_request.hpp>
#include <userver/storages/postgres/cluster.hpp>
namespace ratings_service {
constexpr const char* USER_TICKET_HEADER_NAME = "X-Ratings-User-Ticket";

using TSessionInfo = std::tuple<std::string, std::string,
                                userver::storages::postgres::TimePointTz>;
std::optional<TSessionInfo> GetSessionInfo(
    userver::storages::postgres::ClusterPtr ptr,
    const userver::server::http::HttpRequest& request);
}  // namespace ratings_service