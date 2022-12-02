# Writing Client Application {#ovms_docs_server_app}

@sphinxdirective

.. toctree::
   :maxdepth: 1
   :hidden:
   
   ovms_docs_clients
   ovms_docs_server_api

@endsphinxdirective

OpenVINO&trade; Model Server exposes two sets of APIs: one compatible with TensorFlow Serving and another one, with KServe API, for inference. Both APIs work on both gRPC and REST interfaces. Supporting two sets of APIs makes OpenVINO Model Server easier to plug into existing systems the already leverage one of those APIs for inference. Learn more about supported APIs:

- [TensorFlow Serving gRPC API](./model_server_grpc_api_tfs.md)
- [KServe gRPC API](./model_server_grpc_api_kfs.md)
- [TensorFlow Serving REST API](./model_server_rest_api_tfs.md)
- [KServe REST API](./model_server_rest_api_kfs.md)

In this section you can find short code samples to interact with OpenVINO Model Server endpoints via:
- [TensorFlow Serving API](./clients_tfs.md)
- [KServe API](./clients_kfs.md)

## Sample

This tutorial shows how to perform inference on JPEG images using the gRPC API in OpenVINO Model Server.

## INTRO TO BE ADDED

### Use ovmsclient

To run a prediction using a ResNet-50 image classification model, deploy OpenVINO Model Server with the model. You can do that with the command below:

```bash
docker run -d --rm -p 9000:9000 openvino/model_server:latest \ 
--model_name resnet --model_path gs://ovms-public-eu/resnet50-binary \ 
--layout NHWC:NCHW --port 9000 
```

This command starts the server with a ResNet-50 model downloaded from a public bucket on Google Cloud Storage. With Model Server listening for gRPC calls on port 9000, you can start interacting with the server. Next, let’s install ovmsclient package using pip:

```bash
pip3 install ovmsclient
```
Before running the client, download an image to classify and corresponding ImageNet labels file to interpret prediction results:

```bash
wget
https://raw.githubusercontent.com/openvinotoolkit/model_server/main/demos/common/static/images/zebra...
wget 
https://raw.githubusercontent.com/openvinotoolkit/model_server/main/demos/common/python/classes.py
```
#### Step 1: Create a gRPC connection to the server:

Now you can open Python interpreter and create a gRPC connection to the Model Server.

```bash
$ python3 
Python 3.6.9 (default, Jul 17 2020, 12:50:27) 
[GCC 8.4.0] on linux 
Type "help", "copyright", "credits" or "license" for more information. 
>> from ovmsclient import make_grpc_client 
>> client = make_grpc_client("localhost:9000")
```

#### Step 2: Request the model metadata:

The client object has three methods: get_model_status, get_model_metadata, and predict. To create a valid inference request, you need to know the model input. For that, let’s request model metadata:

```bash
>> client.get_model_metadata(model_name= "resnet") 
{'model_version': 1, 'inputs': {'0': {'shape': [1, 224, 224, 3], 'dtype': 'DT_FLOAT'}}, 'outputs': {'1463': {'shape': [1, 1000], 'dtype': 'DT_FLOAT'}}}
```

#### Step 3: Send a JPEG image to the server:

From model metadata, we learn that the model has one input and one output. The input name is “0” and it expects data in shape (1,224,224,3) with floating point datatype. Now you have all the information needed to run inference. Let’s use the image of a zebra that was downloaded in the previous step. In this example, we will use the Model Server binary inputs feature, which requires just loading a JPEG and requesting prediction on the encoded binary – no preprocessing is needed when using the binary inputs.

```bash
>> with open("zebra.jpeg", "rb") as f: 
... img = f.read() 
... 
>> output = client.predict(inputs={ "0": img}, model_name= "resnet") 
>> type(output) 
<class 'numpy.ndarray'> 
>> output.shape 
(1, 1000) 
```
#### Step 4: Map the output to an imagenet class:

The prediction returned successfully with an output numpy ndarray in shape (1, 1000) – the same as was described in the model metadata in the “outputs” section. The next step is to interpret the model output and extract the classification result.

The shape of the output is (1, 1000), where the first dimension represents batch size (number of processed images) and the second is probability of the image belonging to each of the ImageNet classes. To get the classification result you need to get the index of the maximum value in the second dimension of the output. Then use imagenet_classes dictionary from classes.py downloaded in previous step to perform mapping of the index number to class name and see the result.

```bash
>>> import numpy as np 
>>> from classes import imagenet_classes 
>>> result_index = np.argmax(output[0]) 
>>> imagenet_classes[result_index] 
'zebra' 
```

### Conclusion
The new ovmsclient package is smaller, consumes less memory, and is easier to use than tensorflow-serving-api. In this blog, we learned how to get model metadata and run predictions on binary encoded JPEG image via the gRPC interface in OpenVINO Model Server.

See more detailed examples for running predictions with NumPy arrays, checking model status, and using the REST API on GitHub: https://github.com/openvinotoolkit/model_server/tree/main/client/python/ovmsclient/samples

To learn more about ovmsclient capabilities, see the API documentation:
https://github.com/openvinotoolkit/model_server/blob/main/client/python/ovmsclient/lib/docs/README.m...