cmake_minimum_required(VERSION 3.12)
project(ratings_service CXX)

include(third_party/userver/cmake/SetupEnvironment.cmake)
include(GNUInstallDirs)

add_subdirectory(third_party/userver)

# Common sources
add_library(${PROJECT_NAME}_objs OBJECT
		src/v1/post-a-review/view.cpp
		src/v1/post-a-review/view.hpp
		src/v1/get-reviews/view.cpp
		src/v1/get-reviews/view.hpp
		src/models/review.cpp
		src/models/review.hpp
		src/v1/update-review/view.cpp
		src/v1/update-review/view.hpp
		src/v1/delete-review/view.cpp
		src/v1/delete-review/view.hpp
		src/lib/auth.cpp
		src/lib/auth.hpp
		src/v1/register/view.cpp
		src/v1/register/view.hpp
		src/v1/login/view.cpp
		src/v1/login/view.hpp
		src/v1/log-out/view.cpp
		src/v1/log-out/view.hpp
		src/lib/validation.cpp
		src/lib/validation.hpp)
find_package(Boost REQUIRED COMPONENTS regex	)
target_link_libraries(${PROJECT_NAME}_objs PUBLIC userver-core userver-postgresql Boost::regex)


# The Service
add_executable(${PROJECT_NAME} src/main.cpp)
target_link_libraries(${PROJECT_NAME} PRIVATE ${PROJECT_NAME}_objs)

add_executable(${PROJECT_NAME}_unittest
		unittests/validation.cpp
		)
target_link_libraries(${PROJECT_NAME}_unittest PRIVATE ${PROJECT_NAME}_objs userver-utest)
add_google_tests(${PROJECT_NAME}_unittest)


# Functional Tests
add_subdirectory(tests)

if(DEFINED ENV{PREFIX})
	message(STATUS "Set install prefix: $ENV{PREFIX}")
	file(TO_CMAKE_PATH "$ENV{PREFIX}" PREFIX_PATH)
	set(CMAKE_INSTALL_PREFIX ${PREFIX_PATH})
endif()

set(CONFIG_VAR_PATH ${CMAKE_INSTALL_PREFIX}/${CMAKE_INSTALL_SYSCONFDIR}/${PROJECT_NAME}/config_vars.yaml)
set(CONFIG_FALLBACK_PATH ${CMAKE_INSTALL_PREFIX}/${CMAKE_INSTALL_SYSCONFDIR}/${PROJECT_NAME}/dynamic_config_fallback.json)

configure_file(${CMAKE_CURRENT_SOURCE_DIR}/configs/static_config.yaml.in ${CMAKE_CURRENT_SOURCE_DIR}/configs/static_config.yaml)

FILE(GLOB CONFIGS_FILES ${CMAKE_CURRENT_SOURCE_DIR}/configs/*.yaml ${CMAKE_CURRENT_SOURCE_DIR}/configs/*.json)

install(TARGETS ${PROJECT_NAME} DESTINATION ${CMAKE_INSTALL_BINDIR} COMPONENT ${PROJECT_NAME})
install(FILES ${CONFIGS_FILES} DESTINATION ${CMAKE_INSTALL_SYSCONFDIR}/${PROJECT_NAME} COMPONENT ${PROJECT_NAME})
