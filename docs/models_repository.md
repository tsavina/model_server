# Preparing Model Repository {#ovms_docs_models_repository}

The AI models served by OpenVINO&trade; Model Server must be in either of the three formats:
- [OpenVINO IR](https://docs.openvino.ai/2022.2/openvino_docs_MO_DG_IR_and_opsets.html#doxid-openvino-docs-m-o-d-g-i-r-and-opsets), where the graph is represented in .bin and .xml files 
- [ONNX](https://onnx.ai/), using the .onnx file
- [PaddlePaddle](https://www.paddlepaddle.org.cn/en), using .pdiparams and .pdmodel files

To use models trained in other frameworks you need to convert them first. To do so, use 
OpenVINO [Model Optimizer](https://docs.openvino.ai/2022.2/openvino_docs_MO_DG_Deep_Learning_Model_Optimizer_DevGuide.html) for IR, or different
[converters](https://onnx.ai/supported-tools.html) for ONNX.

The models need to be placed and mounted in a particular directory structure according to the following rules:

- Each model should be stored in a dedicated directory, for example, model1 and model2. 
- Each model directory should include a sub-folder for each of its versions (1,2, and so on). The versions and their folder names should be positive integer values.  
- Every version folder must include model files, that is, .bin and .xml for IR, .onnx for ONNX, .pdiparams and .pdmodel for PaddlePaddle. The file name is arbitrary.

> **NOTE**: In execution, the versions are enabled according to a pre-defined version policy. If the client does not specify the version number in the parameters, by default, the latest version is served.

The structure of the model repository depends on the number of models to be served:
- serving single model
- serving multiple models 

## Serving Single Model

Here is an example structure for a single IR model. 

```
tree models/
models/
└── model1
    └── 1
        ├── ir_model.bin
        └── ir_model.xml
``` 

Start the model server by running the following command: 

```
docker run -d --rm -v <models_repository>:/models -p 9000:9000 -p 9001:9001 openvino/model_server:latest \
--model_path <path_to_model> --model_name <model_name> --port 9000 --rest_port 9001 --log_level DEBUG
```

Example using a resnet model:

```bash
mkdir -p models/resnet/1
wget -P models/resnet/1 https://storage.openvinotoolkit.org/repositories/open_model_zoo/2022.1/models_bin/2/resnet50-binary-0001/FP32-INT1/resnet50-binary-0001.bin
wget -P models/resnet/1 https://storage.openvinotoolkit.org/repositories/open_model_zoo/2022.1/models_bin/2/resnet50-binary-0001/FP32-INT1/resnet50-binary-0001.xml

docker run -d --rm -v ${PWD}/models:/models -p 9000:9000 -p 9001:9001 openvino/model_server:latest \
--model_path /models/resnet/ --model_name resnet --port 9000 --rest_port 9001 --log_level DEBUG
```

@sphinxdirective
.. panels::

    .. dropdown::  Docker container starting parameters 

      +--------------------------------+---------------------------------------------------------------------------------------------------------------------------------+
      | `--rm`                         | | remove the container when exiting the Docker container                                                                        |
      +--------------------------------+---------------------------------------------------------------------------------------------------------------------------------+
      | `-d`                           | | runs the container in the background                                                                                          |
      +--------------------------------+---------------------------------------------------------------------------------------------------------------------------------+
      | `-v`                           | | defines how to mount the model folder in the Docker container                                                                 |
      +--------------------------------+---------------------------------------------------------------------------------------------------------------------------------+
      | `-p`                           | | exposes the model serving port outside the Docker container                                                                   |
      +--------------------------------+---------------------------------------------------------------------------------------------------------------------------------+
      | `openvino/model_server:latest` | | represents the image name; the ovms binary is the Docker entry point                                                          |
      |                                | | varies by tag and build process - see tags: https://hub.docker.com/r/openvino/model_server/tags/ for a full tag list.         |
      +--------------------------------+---------------------------------------------------------------------------------------------------------------------------------+
      | `--model_path`                 | | model location, which can be:                                                                                                 |
      |                                | | a Docker container path that is mounted during start-up                                                                       |
      |                                | | a Google Cloud Storage path `gs://<bucket>/<model_path>`                                                                      |
      |                                | | an AWS S3 path `s3://<bucket>/<model_path>`                                                                                   |
      |                                | | an Azure blob path `az://<container>/<model_path>`                                                                            |
      +--------------------------------+---------------------------------------------------------------------------------------------------------------------------------+
      | `--model_name`                 | | the name of the model in the model_path                                                                                       |
      +--------------------------------+---------------------------------------------------------------------------------------------------------------------------------+
      | `--port`                       | | the gRPC server port                                                                                                          |
      +--------------------------------+---------------------------------------------------------------------------------------------------------------------------------+
      | `--rest_port`                  | | the REST server port                                                                                                          |
      +--------------------------------+---------------------------------------------------------------------------------------------------------------------------------+

@endsphinxdirective


## Serving Multiple Models 

To use a container with several models, you need an additional JSON configuration file defining each model. 

```
tree models/
models/
├── model1
│   ├── 1
│   │   ├── ir_model.bin
│   │   └── ir_model.xml
│   └── 2
│       ├── ir_model.bin
│       └── ir_model.xml
├── model2
│   └── 1
│       ├── ir_model.bin
│       ├── ir_model.xml
│       └── mapping_config.json
├── model3
│    └── 1
│        └── model.onnx
└── model4
     └── 1
         ├── model.pdiparams
         └── model.pdmodel
``` 
In the file, provide a 
`model_config_list` array that includes a collection of config objects for each served model. The `name` and the `base_path` values of the model are required for every config object.


@sphinxdirective
.. panels::

    .. dropdown::  Config example

      ```json
      {
         "model_config_list":[
            {
               "config":{
                  "name":"model_name1",
                  "base_path":"/opt/ml/models/model1",
                  "batch_size": "16"
               }
            },
            {
               "config":{
                  "name":"model_name2",
                  "base_path":"/opt/ml/models/model2",
                  "batch_size": "auto",
                  "model_version_policy": {"all": {}}
               }
            },
            {
               "config":{
                  "name":"model_name3",
                  "base_path":"gs://bucket/models/model3",
                  "model_version_policy": {"specific": { "versions":[1, 3] }},
                  "shape": "auto"
               }
            },
            {
               "config":{
                  "name":"model_name4",
                  "base_path":"s3://bucket/models/model4",
                  "shape": {
                     "input1": "(1,3,200,200)",
                     "input2": "(1,3,50,50)"
                  },
                  "plugin_config": {"CPU_THROUGHPUT_STREAMS": "CPU_THROUGHPUT_AUTO"}
               }
            },
            {
               "config":{
                  "name":"model_name5",
                  "base_path":"s3://bucket/models/model5",
                  "shape": "auto",
                  "nireq": 32,
                  "target_device": "HDDL"
               }
            }
         ]
      }
      ```
@endsphinxdirective


When the Docker container has the config file mounted, it can be started - the command is minimalistic, as arguments are read from the config file. 

The models also can be hosted remotely by cloud storages, including Google Cloud Storage (GCS), Amazon S3, or Azure Blob Storage. Learn how to [use cloud storage as a model repository](using_cloud_storage.md).

## Next Steps

- Learn how to serve [single](single_model_mode.md) or [multiple](multiple_models_mode.md) models.
- After the model is ready for serving, proceed to [start the model server](docker_container.md).
- [Use cloud storage](using_cloud_storage.md) as a model repository.

## Additional Resources

- [Quickstart guide](./ovms_quickstart.md)
- [Model server starting parameters](parameters.md)
- [Troubleshooting](troubleshooting.md)