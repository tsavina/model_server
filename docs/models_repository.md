# Preparing Model Repository and Serving Models {#ovms_docs_models_repository}

The AI models served by OpenVINO&trade; Model Server must be in either of the three formats:
- [OpenVINO IR](https://docs.openvino.ai/2022.2/openvino_docs_MO_DG_IR_and_opsets.html#doxid-openvino-docs-m-o-d-g-i-r-and-opsets), where the graph is represented in .bin and .xml files 
- [ONNX](https://onnx.ai/), using the .onnx file
- [PaddlePaddle](https://www.paddlepaddle.org.cn/en), using .pdiparams and .pdmodel files

To use models trained in other frameworks you need to convert them first. To do so, use 
OpenVINO [Model Optimizer](https://docs.openvino.ai/2022.2/openvino_docs_MO_DG_Deep_Learning_Model_Optimizer_DevGuide.html) for IR, or different
[converters](https://onnx.ai/supported-tools.html) for ONNX.

The models need to be placed and mounted in a particular directory structure according to the following rules:

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

- Each model should be stored in a dedicated directory, for example, model1 and model2. 
- Each model directory should include a sub-folder for each of its versions (1,2, and so on). The versions and their folder names should be positive integer values.  
- Every version folder must include model files, that is, .bin and .xml for IR, .onnx for ONNX, .pdiparams and .pdmodel for PaddlePaddle. The file name is arbitrary.

> **NOTE**: In execution, the versions are enabled according to a pre-defined version policy. If the client does not specify the version number in the parameters, by default, the latest version is served.

The models also can be hosted remotely by cloud storages, including Google Cloud Storage (GCS), Amazon S3, or Azure Blob Storage. Learn how to [use cloud storage as a model repository](using_cloud_storage.md).

## Serving Models 

### Serving Single Model

Before starting the container, make sure you [prepared the model for serving](models_repository.md).

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
.. raw:: html
    <div class="collapsible-section" data-title="Configuration Arguments for Running Model Server: Click to expand/collapse">
@endsphinxdirective


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

@sphinxdirective
.. raw:: html
    </div>
@endsphinxdirective

- Publish the container's port to your host's **open ports**. 
- In the command above, port 9000 is exposed for gRPC and port 9001 is exposed for REST API calls.
- Add model_name for the client gRPC/REST API calls.

### Serving Multiple Models 

To use a container with several models, you need an additional JSON configuration file defining each model. In the file, provide a 
`model_config_list` array that includes a collection of config objects for each served model. The `name` and the `base_path` values of the model are required for every config object.

@sphinxdirective
.. raw:: html
    <div class="collapsible-section" data-title="An example of the configuration file: Click to expand/collapse">
@endsphinxdirective

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

@sphinxdirective
.. raw:: html
    </div>
@endsphinxdirective

When the Docker container has the config file mounted, it can be started - the command is minimalistic, as arguments are read from the config file. 
Note that models with a cloud storage path require setting specific environmental variables.

```

docker run --rm -d -v /models/:/opt/ml:ro -p 9001:9001 -p 8001:8001 -v <config.json>:/opt/ml/config.json openvino/model_server:latest \
--config_path /opt/ml/config.json --port 9001 --rest_port 8001

```

### Setting Input and Output Tensors

Each model defines input and output tensors in the model graph. The client passes data to model input tensors by filling appropriate entries in the request input map. 
Prediction results can be read from the response output map. By default, OpenVINO™ Model Server uses model tensor names as input and output names in 
prediction requests and responses. The client passes the input values to the request and reads the results by referring to the corresponding output names.

Here is an example of client code:

```python
input_tensorname = 'input'
request.inputs[input_tensorname].CopyFrom(make_tensor_proto(img, shape=(1, 3, 224, 224)))

.....

output_tensorname = 'resnet_v1_50/predictions/Reshape_1'
predictions = make_ndarray(result.outputs[output_tensorname])
```


It is possible to adjust this behavior by adding an optional .json file named `mapping_config.json`. 
It can map the input and output keys to the appropriate tensors. This extra mapping can be used to enable user-friendly names for models with difficult tensor names.
Here is an example of mapping_config.json:

```json
{
       "inputs":{ 
          "tensor_name":"grpc_custom_input_name"
       },
       "outputs":{
          "tensor_name1":"grpc_output_key_name1",
          "tensor_name2":"grpc_output_key_name2"
       }
}
```

## Next Steps

- Learn how to serve [single](single_model_mode.md) or [multiple](multiple_models_mode.md) models.
- After the model is ready for serving, proceed to [start the model server](docker_container.md).
- [Use cloud storage](using_cloud_storage.md) as a model repository.

## Additional Resources

- [Quickstart guide](./ovms_quickstart.md)
- [Model server starting parameters](parameters.md)
- [Troubleshooting](troubleshooting.md)