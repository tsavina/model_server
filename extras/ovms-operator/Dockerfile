#
# Copyright (c) 2021 Intel Corporation
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

FROM quay.io/operator-framework/helm-operator:v1.4.0
LABEL "name"="ovms-operator"
LABEL "vendor"="Intel Corporation"
LABEL "version"="0.1.0"
LABEL "release"="0.1"
LABEL "summary"="OpenVINO(TM) Model Server Operator"
LABEL "description"="An Operator for managing OpenVINO Model Server in Kubernetes and OpenShift"
ENV HOME=/opt/helm
COPY watches.yaml ${HOME}/watches.yaml
COPY helm-charts  ${HOME}/helm-charts
COPY LICENSE /licenses/LICENSE
WORKDIR ${HOME}
