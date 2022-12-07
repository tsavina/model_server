# Additional Configurations {#ovms_docs_additional_configurations}

@sphinxdirective

.. toctree::
   :maxdepth: 1
   :hidden:

   ovms_docs_parameters
   ovms_docs_target_devices
   ovms_docs_cloud_storage

@endsphinxdirective

## Configuring Deployment

Depending on performance requirements, traffic expectations, and  models, you may want to make certain adjustments to:  

configuration of server options:
- ports used
- enable/disable REST API
- set configuration monitoring 

configuration for each of the served models:  
- the device to load the model onto
- the model version policy
- inference related options

Read about the [model server parameters](parameters.md) to get more details on the model server configuration. 

## Using AI Accelerators

Learn how to [configure AI accelerators](accelerators.md), such as Intel Movidius Myriad VPUs, 
GPU, and HDDL, as well as Multi-Device, Heterogeneous and Auto Device Plugins for inference execution. 

## Keeping Models in a Remote Storage

Leverage remote storages, compatible with Google Cloud Storage (GCS), Amazon S3, or Azure Blob Storage, to create more flexible model repositories 
that are easy to use and manage, for example, in Kubernetes deployments. [Learn more](using_cloud_storage.md)