#include <userver/clients/http/component.hpp>
#include <userver/components/minimal_server_component_list.hpp>
#include <userver/server/handlers/ping.hpp>
#include <userver/server/handlers/tests_control.hpp>
#include <userver/testsuite/testsuite_support.hpp>
#include <userver/utils/daemon_run.hpp>

#include "./v1/get-reviews/view.hpp"
#include "./v1/post-a-review/view.hpp"
#include "./v1/update-review/view.hpp"
#include "hello.hpp"
int main(int argc, char* argv[]) {
  auto component_list = userver::components::MinimalServerComponentList()
                            .Append<userver::server::handlers::Ping>()
                            .Append<userver::components::TestsuiteSupport>()
                            .Append<userver::components::HttpClient>()
                            .Append<userver::server::handlers::TestsControl>();

  ratings_service::AppendHello(component_list);
  ratings_service::AppendPostReview(component_list);
  ratings_service::AppendGetReviews(component_list);
  ratings_service::AppendUpdateReview(component_list);
  return userver::utils::DaemonMain(argc, argv, component_list);
}
