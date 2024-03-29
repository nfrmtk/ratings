#include <userver/clients/dns/component.hpp>
#include <userver/clients/http/component.hpp>
#include <userver/components/minimal_server_component_list.hpp>
#include <userver/server/handlers/ping.hpp>
#include <userver/server/handlers/tests_control.hpp>
#include <userver/storages/postgres/component.hpp>
#include <userver/testsuite/testsuite_support.hpp>
#include <userver/utils/daemon_run.hpp>
#include "./v1/delete-review/view.hpp"
#include "./v1/get-reviews/view.hpp"
#include "./v1/log-out/view.hpp"
#include "./v1/login/view.hpp"
#include "./v1/post-a-review/view.hpp"
#include "./v1/register/view.hpp"
#include "./v1/update-review/view.hpp"
int main(int argc, char* argv[]) {
  auto component_list =
      userver::components::MinimalServerComponentList()
          .Append<userver::server::handlers::Ping>()
          .Append<userver::components::TestsuiteSupport>()
          .Append<userver::components::HttpClient>()
          .Append<userver::server::handlers::TestsControl>()
          .Append<userver::clients::dns::Component>()
          .Append<userver::components::Postgres>("postgres-db-1");

  ratings_service::AppendPostReview(component_list);
  ratings_service::AppendGetReviews(component_list);
  ratings_service::AppendUpdateReview(component_list);
  ratings_service::AppendDeleteReview(component_list);
  ratings_service::AppendRegister(component_list);
  ratings_service::AppendLogin(component_list);
  ratings_service::AppendLogOut(component_list);
  return userver::utils::DaemonMain(argc, argv, component_list);
}
