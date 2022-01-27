//*****************************************************************************
// Copyright 2022 Intel Corporation
//
// Licensed under the Apache License, Version 2.0 (the "License");
// you may not use this file except in compliance with the License.
// You may obtain a copy of the License at
//
//     http://www.apache.org/licenses/LICENSE-2.0
//
// Unless required by applicable law or agreed to in writing, software
// distributed under the License is distributed on an "AS IS" BASIS,
// WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
// See the License for the specific language governing permissions and
// limitations under the License.
//*****************************************************************************
#include <utility>

#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include "../tensorinfo.hpp"

using namespace ovms;

TEST(TensorInfo, Intersection) {
    //  match
    auto first = std::make_shared<TensorInfo>("a", "b", Precision::FP32, Shape({1, 3, 224, {220, 230}}), Layout{"NCHW"});
    auto second = std::make_shared<TensorInfo>("a", "b", Precision::FP32, Shape({1, Dimension::any(), {220, 225}, {200, 300}}), Layout{"NCHW"});
    auto intersect = first->createIntersection(*second);  // TODO create expected TensorInfo
    ASSERT_NE(intersect, nullptr);
    EXPECT_TRUE(intersect->isTensorSpecEqual(TensorInfo("a", "b", Precision::FP32, Shape({1, 3, 224, {220, 230}}), Layout{"NCHW"}))) << intersect->asString();  // TODO create expected TensorInfo
    // precision UNDEFINED
    first = std::make_shared<TensorInfo>("a", "b", Precision::FP32, Shape({1, 3, 224, {220, 230}}), Layout{"NCHW"});
    second = std::make_shared<TensorInfo>("a", "b", Precision::UNDEFINED, Shape({1, 3, 224, {220, 230}}), Layout{"NCHW"});
    intersect = first->createIntersection(*second);  // TODO create expected TensorInfo
    ASSERT_NE(intersect, nullptr);
    EXPECT_TRUE(intersect->isTensorSpecEqual(TensorInfo("a", "b", Precision::FP32, Shape({1, 3, 224, {220, 230}}), Layout{"NCHW"}))) << intersect->asString();  // TODO create expected TensorInfo
    // Unspecified intersection should succeed with any & itself
    first = TensorInfo::getUnspecifiedTensorInfo();
    second = TensorInfo::getUnspecifiedTensorInfo();
    intersect = first->createIntersection(*second);
    ASSERT_NE(intersect, nullptr);
    EXPECT_TRUE(intersect->isTensorSpecEqual(*first)) << intersect->asString();  // TODO create expected TensorInfo
    // uspecified should succeed with any
    first = TensorInfo::getUnspecifiedTensorInfo();
    second = std::make_shared<TensorInfo>("a", "b", Precision::FP32, Shape({1, Dimension::any(), {220, 225}, {200, 300}}), Layout{"NCHW"});
    intersect = first->createIntersection(*second);
    ASSERT_NE(intersect, nullptr);
    EXPECT_TRUE(intersect->isTensorSpecEqual(*second)) << intersect->asString();  // TODO create expected TensorInfo
    // default layout should match any // TODO this is intentional for now until stricter layout checking implementation
    first = std::make_shared<TensorInfo>("a", "b", Precision::FP32, Shape({1, Dimension::any(), {220, 225}, {200, 300}}), TensorInfo::getDefaultLayout());
    second = std::make_shared<TensorInfo>("a", "b", Precision::FP32, Shape({1, Dimension::any(), {220, 225}, {200, 300}}), Layout{"NCHW"});
    intersect = first->createIntersection(*second);
    ASSERT_NE(intersect, nullptr);
    EXPECT_TRUE(intersect->isTensorSpecEqual(*second)) << intersect->asString();  // TODO create expected TensorInfo
    // precision mismatch
    first = std::make_shared<TensorInfo>("a", "b", Precision::FP32, Shape({1, 3, 224, 224}), Layout{"NCHW"});
    second = std::make_shared<TensorInfo>("a", "b", Precision::I32, Shape({1, 3, 224, 224}), Layout{"NCHW"});
    EXPECT_EQ(first->createIntersection(*second), nullptr);
    // layout order mismatch // TODO
    first = std::make_shared<TensorInfo>("a", "b", Precision::FP32, Shape({1, 3, 224, 224}), Layout{"NCHW"});
    second = std::make_shared<TensorInfo>("a", "b", Precision::FP32, Shape({1, 3, 224, 224}), Layout{"NHWC"});
    EXPECT_EQ(first->createIntersection(*second), nullptr);
    // name mismatch // TODO
    first = std::make_shared<TensorInfo>("a", "b", Precision::FP32, Shape({1, 3, 224, 224}), Layout{"NCHW"});
    second = std::make_shared<TensorInfo>("a2", "b", Precision::FP32, Shape({1, 3, 224, 224}), Layout{"NCHW"});
    EXPECT_EQ(first->createIntersection(*second), nullptr);
    // mapped name mismatch // TODO
    first = std::make_shared<TensorInfo>("a", "b", Precision::FP32, Shape({1, 3, 224, 224}), Layout{"NCHW"});
    second = std::make_shared<TensorInfo>("a", "b2", Precision::FP32, Shape({1, 3, 224, 224}), Layout{"NCHW"});
    EXPECT_EQ(first->createIntersection(*second), nullptr);
}
