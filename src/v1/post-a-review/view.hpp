#pragma once

#include <userver/components/component_list.hpp>

namespace ratings_service {

void AppendPostReview(userver::components::ComponentList& components);
}