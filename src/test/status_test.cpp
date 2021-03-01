//*****************************************************************************
// Copyright 2021 Intel Corporation
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

#include <gmock/gmock.h>
#include <gtest/gtest.h>

#include "../status.hpp"

using testing::ElementsAre;

using namespace ovms;

TEST(Status, AllStatusesHaveMappedString) {
    // todo cleanup statuses from here
    std::vector<StatusCode> whitelist{};
    uint32_t unknownCodesCount = 0;
    for(uint32_t code = static_cast<uint32_t>(StatusCode::OK); code < static_cast<uint32_t>(StatusCode::STATUS_CODE_END); ++code) {
        if (whitelist.end() != std::find(whitelist.begin(), whitelist.end(), StatusCode(code))) {
            continue;
        }
        EXPECT_NE(Status(static_cast<StatusCode>(code)).string(), "Unknown error") << "Code:" << code << " does not have assigned mapping string";
        if (Status(static_cast<StatusCode>(code)).string() == "Unknown error") {
            // brokenStatuses.emplace_back(code);
            ++unknownCodesCount;
        }
    }
    for (auto& statusCode : whitelist) {
        // This will signal that some status code was fixed and shouldn;t be anymore on whitelist
        EXPECT_EQ(Status(statusCode).string(), "Unknown error") << "Code: " << static_cast<uint32_t>(statusCode) << " should be removed from whitelist";
    }
    EXPECT_EQ(unknownCodesCount, 0);
}

