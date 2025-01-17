//*****************************************************************************
// Copyright 2020 Intel Corporation
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

#include "schema.hpp"

#include <string>

#include <rapidjson/error/en.h>
#include <rapidjson/error/error.h>
#include <rapidjson/schema.h>
#include <rapidjson/stringbuffer.h>
#include <spdlog/spdlog.h>

namespace ovms {
const char* MODELS_CONFIG_SCHEMA = R"({
	"definitions": {
		"custom_loader_config": {
			"type": "object",
			"required": ["config"],
			"properties": {
				"config": {
					"type": "object",
					"required": ["loader_name", "library_path"],
					"properties": {
						"loader_name": {
							"type": "string"
						},
						"library_path": {
							"type": "string"
						},
						"loader_config_file": {
							"type": "string"
						}
					},
					"additionalProperties": false
				},
				"additionalProperties": false
			}
		},
		"model_config": {
			"type": "object",
			"required": ["config"],
			"properties": {
				"config": {
					"type": "object",
					"required": ["name", "base_path"],
					"properties": {
						"name": {
							"type": "string"
						},
						"base_path": {
							"type": "string"
						},
						"batch_size": {
							"type": ["integer", "string"],
							"minimum": 0
						},
						"model_version_policy": {
							"type": "object"
						},
						"shape": {
							"type": ["object", "string"]
						},
						"layout": {
							"type": ["object", "string"]
						},
						"nireq": {
							"type": "integer",
							"minimum": 0
						},
						"target_device": {
							"type": "string"
						},
                        "allow_cache": {
                            "type": "boolean"
                        },
						"plugin_config": {
							"type": "object"
						},
						"stateful": {
							"type": "boolean"
						},
						"idle_sequence_cleanup": {
							"type": "boolean"
						},
						"low_latency_transformation": {
							"type": "boolean"
						},
						"max_sequence_number": {
							"type": "integer",
							"minimum": 0
						},
						"custom_loader_options": {
							"type": "object",
                                                        "required": ["loader_name"],
                                                        "properties": {
                                                            "loader_name": {
                                                                "type": "string"
                                                            }
                                                        },
                                                        "minProperties": 1
						}
					},
					"additionalProperties": false
				},
				"additionalProperties": false
			}
		},
		"source_node_names": {
			"type": "object",
			"required": ["node_name", "data_item"],
			"properties": {
				"node_name": {
					"type": "string"
				},
				"data_item": {
					"type": "string"
				}
			},
			"additionalProperties": false
		},
		"source_node": {
			"type": "object",
			"additionalProperties" : {
				"$ref": "#/definitions/source_node_names"
			},
			"minProperties": 1,
			"maxProperties": 1
		},
		"output_alias": {
			"type": "object",
			"required": ["data_item", "alias"],
			"properties": {
				"data_item": {
					"type": "string"
				},
				"alias": {
					"type": "string"
				}
			},
			"additionalProperties": false
		},
		"node_config": {
			"type": "object",
			"required": ["name", "type", "inputs", "outputs"],
			"oneOf": [
    			{
        			"properties": { "type": { "enum": ["custom"] } },
        			"required": ["library_name"],
					"not": { "required": ["model_name"] }
    			},
    			{
        			"properties": { "type": { "enum": ["DL model"] } },
        			"not": { "required": ["library_name"] },
					"required": ["model_name"]
    			}
  			],
			"properties": {
				"name": {
					"type": "string"
				},
				"model_name": {
					"type": "string"
				},
				"library_name": {
					"type": "string"
				},
				"type": {
					"type": "string",
					"enum": ["DL model", "custom"]
				},
				"version": {
					"type": "integer",
					"minimum": 1
				},
				"inputs": {
					"type": "array",
					"items": {
						"$ref": "#/definitions/source_node"
					}
				},
				"outputs": {
					"type": "array",
					"items": {
						"$ref": "#/definitions/output_alias"
					}
				},
				"params": {
					"type": "object",
					"additionalProperties": { "type": "string" } 
				},
				"demultiply_count": {
			"type": "integer",
			"minimum": -1,
			"maximum": 10000
				},
				"gather_from_node": {
					"type": "string"
				}
			},
			"additionalProperties": false
		},
		"pipeline_config": {
			"type": "object",
			"required": ["name", "nodes", "inputs", "outputs"],
			"properties": {
				"name": {
					"type": "string"
				},
				"nodes": {
					"type": "array",
					"items": {
						"$ref": "#/definitions/node_config"
					}
				},
				"inputs": {
					"type": "array",
					"items": {
						"type": "string"
					}
				},
				"outputs": {
					"type": "array",
					"items": {
						"$ref": "#/definitions/source_node"
					}
				},
        "demultiply_count" : {
			"type": "integer",
			"minimum": -1,
			"maximum": 10000
        }
			},
			"additionalProperties": false
		},
		"custom_node_library_config": {
			"type": "object",
			"required": ["name", "base_path"],
			"properties": {
				"name": {
					"type": "string"
				},
				"base_path": {
					"type": "string"
				}
			},
			"additionalProperties": false
		}
	},
	"type": "object",
	"required": ["model_config_list"],
	"properties": {
		"custom_loader_config_list": {
			"type": "array",
			"items": {
				"$ref": "#/definitions/custom_loader_config"
			}
		},
		"model_config_list": {
			"type": "array",
			"items": {
				"$ref": "#/definitions/model_config"
			}
		},
		"pipeline_config_list": {
			"type": "array",
			"items": {
				"$ref": "#/definitions/pipeline_config"
			}
		},
		"custom_node_library_config_list": {
			"type": "array",
			"items": {
				"$ref": "#/definitions/custom_node_library_config"
			}
		},
		"monitoring": {
			"type": "object",
			"required": ["metrics"],
			"properties":{
				"metrics": {
					"type": "object",
					"required": ["enable"],
					"properties": {
						"enable": {
							"type": "boolean"
						},
						"metrics_list": {
							"type": "array",
							"items": {
								"type": "string"
							}
						}
					},
					"additionalProperties": false
				},
				"additionalProperties": false
			},
			"additionalProperties": false
		}
	},
	"additionalProperties": false
})";

const char* MODELS_MAPPING_INPUTS_SCHEMA = R"({
    "type": "object",
    "required": [
        "inputs"
    ],
    "properties": {
		"outputs":{
            "type": "object"
        },
        "inputs":{
            "type": "object"
        }
    },
	"additionalProperties": false
    })";

const char* MODELS_MAPPING_OUTPUTS_SCHEMA = R"({
    "type": "object",
    "required": [
        "outputs"
    ],
    "properties": {
        "outputs":{
            "type": "object"
        },
        "inputs":{
            "type": "object"
        }
    },
	"additionalProperties": false
    })";

StatusCode validateJsonAgainstSchema(rapidjson::Document& json, const char* schema) {
    rapidjson::Document schemaJson;
    rapidjson::ParseResult parsingSucceeded = schemaJson.Parse(schema);
    if (!parsingSucceeded) {
        SPDLOG_ERROR("JSON schema parse error: {}, at: {}", rapidjson::GetParseError_En(parsingSucceeded.Code()), parsingSucceeded.Offset());
        return StatusCode::JSON_INVALID;
    }
    rapidjson::SchemaDocument parsedSchema(schemaJson);
    rapidjson::SchemaValidator validator(parsedSchema);
    if (!json.Accept(validator)) {
        rapidjson::StringBuffer sb;
        validator.GetInvalidSchemaPointer().StringifyUriFragment(sb);
        std::string schema = sb.GetString();
        std::string keyword = validator.GetInvalidSchemaKeyword();
        sb.Clear();
        validator.GetInvalidDocumentPointer().StringifyUriFragment(sb);
        std::string key = sb.GetString();

        SPDLOG_ERROR("Given config is invalid according to schema: {}. Keyword: {} Key: {}", schema, keyword, key);
        return StatusCode::JSON_INVALID;
    }

    return StatusCode::OK;
}

}  // namespace ovms
