#
# Copyright (c) 2020 Intel Corporation
#
# Licensed under the Apache License, Version 2.0 (the "License");
# you may not use this file except in compliance with the License.
# You may obtain a copy of the License at
#
#      http://www.apache.org/licenses/LICENSE-2.0
#
# Unless required by applicable law or agreed to in writing, software
# distributed under the License is distributed on an "AS IS" BASIS,
# WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
# See the License for the specific language governing permissions and
# limitations under the License.
#
cmake_minimum_required(VERSION 2.8)
include(FetchContent)
project(ovms_test)
set(CMAKE_CXX_STANDARD 17)
SET(CMAKE_CXX_FLAGS "-g -O0 -Wall -fprofile-arcs -ftest-coverage")
SET(CMAKE_C_FLAGS "-g -O0 -Wall -W -fprofile-arcs -ftest-coverage")
SET(CMAKE_EXE_LINKER_FLAGS "-fprofile-arcs -ftest-coverage")

file(GLOB ovms_test_SRC
    "test/*.hpp"
    "test/*.cpp"
)

add_executable(ovms_test ${ovms_test_SRC})

add_library(libsampleloader SHARED example/SampleCustomLoader/sampleCustLoader.cpp customloaderinterface.hpp)

add_library(lib_node_mock SHARED test/custom_nodes/node_mock.cpp custom_node_interface.hpp)

add_library(lib_node_missing_implementation SHARED test/custom_nodes/node_missing_implementation.cpp custom_node_interface.hpp)

################################
# GTest
################################
set(GTEST_VERSION 1.10.0 CACHE STRING "Google test version")
FetchContent_Declare(googletest
GIT_REPOSITORY https://github.com/google/googletest.git
GIT_TAG release-${GTEST_VERSION})

FetchContent_GetProperties(googletest)
if(NOT googletest_POPULATED)
  FetchContent_Populate(googletest)
  add_subdirectory(${googletest_SOURCE_DIR} ${googletest_BINARY_DIR})
endif()

enable_testing()

set(BAZEL_BUILD "$ENV{BAZEL_BUILD}")
include_directories($<TARGET_PROPERTY:gtest,INTERFACE_SYSTEM_INCLUDE_DIRECTORIES>
                      $<TARGET_PROPERTY:gtest_main,INTERFACE_SYSTEM_INCLUDE_DIRECTORIES>
                      $<TARGET_PROPERTY:gmock,INTERFACE_SYSTEM_INCLUDE_DIRECTORIES>)
include_directories(/root/.cache/bazel/_bazel_root/${BAZEL_BUILD}/external/spdlog/include/)
include_directories(/root/.cache/bazel/_bazel_root/${BAZEL_BUILD}/external/com_github_grpc_grpc/include/)
include_directories(/root/.cache/bazel/_bazel_root/${BAZEL_BUILD}/external/tensorflow_serving/)
include_directories(/root/.cache/bazel/_bazel_root/${BAZEL_BUILD}/external/cxxopts/include/)
include_directories(/root/.cache/bazel/_bazel_root/${BAZEL_BUILD}/external/eigen_archive/)
include_directories(/root/.cache/bazel/_bazel_root/${BAZEL_BUILD}/external/org_tensorflow/)
include_directories(/root/.cache/bazel/_bazel_root/${BAZEL_BUILD}/external/com_google_absl/)
include_directories(/root/.cache/bazel/_bazel_root/${BAZEL_BUILD}/execroot/ovms/bazel-out/k8-opt/bin/external/org_tensorflow/)
include_directories(/root/.cache/bazel/_bazel_root/${BAZEL_BUILD}/external/com_google_protobuf/src/)
include_directories(/root/.cache/bazel/_bazel_root/${BAZEL_BUILD}/execroot/ovms/bazel-out/k8-opt/bin/external/tensorflow_serving/)
include_directories(/root/.cache/bazel/_bazel_root/${BAZEL_BUILD}/external/rapidjson/include/)
include_directories(/root/.cache/bazel/_bazel_root/${BAZEL_BUILD}/external/com_github_googleapis_google_cloud_cpp/)
include_directories(/root/.cache/bazel/_bazel_root/${BAZEL_BUILD}/external/com_github_googleapis_google_cloud_cpp_common/)
include_directories(/root/.cache/bazel/_bazel_root/${BAZEL_BUILD}/execroot/ovms/bazel-out/k8-opt/bin/external/com_github_googleapis_google_cloud_cpp/google/cloud/storage/)
include_directories(/root/.cache/bazel/_bazel_root/${BAZEL_BUILD}/execroot/ovms/bazel-out/k8-opt/bin/external/awssdk/_virtual_includes/core/)
include_directories(/awssdk/aws-cpp-sdk-s3/include)

find_package(InferenceEngine REQUIRED)
find_package(ngraph REQUIRED)

target_include_directories(
    ${PROJECT_NAME}
    PRIVATE
    ${InferenceEngine_DIR}/../include
    ${InferenceEngine_DIR}/../include/cpp
    ${NGraph_DIR}/../include
)

message(STATUS ${NGRAPH_LIBRARIES})

file(STRINGS ovms_libs.in OVMS_LIBS)

target_link_libraries(${PROJECT_NAME} PRIVATE ${InferenceEngine_LIBRARIES} ${OpenCV_LIBS} ${NGRAPH_LIBRARIES} )
target_link_libraries(${PROJECT_NAME} azurestorage cpprest)
target_link_libraries(${PROJECT_NAME} ${OVMS_LIBS})
target_link_libraries(${PROJECT_NAME} stdc++fs ssl crypto uuid xml2 dl)
target_link_libraries(${PROJECT_NAME} libsampleloader lib_node_mock lib_node_missing_implementation gtest gtest_main gmock)

SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++17 -pthread")
