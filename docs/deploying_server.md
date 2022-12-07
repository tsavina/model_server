# Starting Model Server {#ovms_docs_deploying_server}

1. Docker is the recommended way to deploy OpenVINO Model Server. Pre-built container images are available on Docker Hub and Red Hat Ecosystem Catalog. 
2. Build a Docker container from source, if you want to have full control of the container.
3. Deploy OpenVINO Model Server in Kubernetes via helm chart, Kubernetes Operator or OpenShift Operator.

## Deploying Model Server in Docker Container 

This is a step-by-step guide on how to deploy OpenVINO&trade; Model Server on Linux, using a pre-build Docker Container. 

**Before you start, make sure you have:**

- [Docker Engine](https://docs.docker.com/engine/) installed 
- Intel® Core™ processor (6-12th gen.) or Intel® Xeon® processor
- Linux, macOS or Windows via [WSL](https://docs.microsoft.com/en-us/windows/wsl/) 
- (optional) AI accelerators [supported by OpenVINO](https://docs.openvino.ai/2022.2/openvino_docs_IE_DG_supported_plugins_Supported_Devices.html). Accelerators are tested only on bare-metal Linux hosts.

### Launch Model Server Container 

This example shows how to launch the model server with a ResNet50 image classification model from a cloud storage:

#### Step 1. Pull Model Server Image

Pull an image from Docker: 

```bash
docker pull openvino/model_server:latest
```

or [RedHat Ecosystem Catalog](https://catalog.redhat.com/software/containers/intel/openvino-model-server/607833052937385fc98515de):

```
docker pull registry.connect.redhat.com/intel/openvino-model-server:latest
```

#### Step 2. Prepare Data for Serving

```bash
# start the container with the model
docker run -p 9000:9000 openvino/model_server:latest \ 
--model_name resnet --model_path gs://ovms-public-eu/resnet50-binary \ 
--layout NHWC:NCHW --port 9000 

# download input files: an image and a label mapping file
wget https://raw.githubusercontent.com/openvinotoolkit/model_server/releases/2022/1/demos/common/static/images/zebra.jpeg
wget https://raw.githubusercontent.com/openvinotoolkit/model_server/releases/2022/1/demos/common/python/classes.py

# install the Python-based ovmsclient package
pip3 install ovmsclient
```

#### Step 3. Run Prediction


```bash
echo 'import numpy as np
from classes import imagenet_classes
from ovmsclient import make_grpc_client

client = make_grpc_client("localhost:9000")

with open("zebra.jpeg", "rb") as f:
   img = f.read()

output = client.predict({"0": img}, "resnet")
result_index = np.argmax(output[0])
print(imagenet_classes[result_index])' >> predict.py

python predict.py
```
If everything is set up correctly, you will see 'zebra' prediction in the output.

## Building Container from Source 

Before starting the server, make sure your hardware is [supported](https://docs.openvino.ai/2022.2/_docs_IE_DG_supported_plugins_Supported_Devices.html) by OpenVINO.

> **NOTE**: OpenVINO Model Server execution on baremetal is tested on Ubuntu 20.04.x. For other operating systems, starting model server in a [docker container](./docker_container.md) is recommended.
   
1. Clone model server git repository.
2. Navigate to the model server directory.
3. Use a precompiled binary or build it in a Docker container.
4. Navigate to the folder containing the binary package and unpack the `tar.gz` file.

Run the following commands to build a model server Docker image:

```bash

git clone https://github.com/openvinotoolkit/model_server.git

cd model_server   
   
# automatically build a container from source
# it places a copy of the binary package in the `dist` subfolder in the Model Server root directory
make docker_build

# unpack the `tar.gz` file
cd dist/ubuntu && tar -xzvf ovms.tar.gz

```
In the `./dist` directory it will generate: 

- image tagged as openvino/model_server:latest - with CPU, NCS, and HDDL support
- image tagged as openvino/model_server:latest-gpu - with CPU, NCS, HDDL, and iGPU support
- image tagged as openvino/model_server:latest-nginx-mtls - with CPU, NCS, and HDDL support and a reference nginx setup of mTLS integration
- release package (.tar.gz, with ovms binary and necessary libraries)

> **NOTE**: Model Server docker image can be created with ubi8-minimal base image or the default ubuntu20. Model Server with the ubi base image does not support NCS and HDDL accelerators.

### Running the Server

The server can be started in two ways:

- using the ```./ovms/bin/ovms --help``` command in the folder, where OVMS is installed
- in the interactive mode - as a background process or a daemon initiated by ```systemctl/initd``` depending on the Linux distribution and specific hosting requirements

Learn more about model server [starting parameters](parameters.md).

## Deploying Model Server in Kubernetes 

There are three recommended methods for deploying OpenVINO Model Server in Kubernetes:
1. [helm chart](https://github.com/openvinotoolkit/operator/tree/main/helm-charts/ovms) - deploys Model Server instances using the [helm](https://helm.sh) package manager for Kubernetes

2. [Kubernetes Operator](https://operatorhub.io/operator/ovms-operator) - manages Model Server using a Kubernetes Operator
3. [OpenShift Operator](https://github.com/openvinotoolkit/operator/blob/main/docs/operator_installation.md#openshift) - manages Model Server instances in Red Hat OpenShift

For operators mentioned in 2. and 3. see the [description of the deployment process](https://github.com/openvinotoolkit/operator/blob/main/docs/modelserver.md)

## Next Steps

- Try the model server [features](features.md)
- Explore the model server [demos](../demos/README.md)

## Additional Resources

- [Preparing Model Repository](models_repository.md)
- [Using Cloud Storage](using_cloud_storage.md)
- [Troubleshooting](troubleshooting.md)
- [Model server parameters](parameters.md)