#include <inference_engine.hpp>

int main() {
    InferenceEngine::Core ie;
    InferenceEngine::CNNNetwork network = ie.ReadNetwork("src/test/dummy/1/dummy.xml");
    InferenceEngine::ExecutableNetwork executable_network = ie.LoadNetwork(network, "CPU");
    auto ireq = executable_network.CreateInferRequest();

    InferenceEngine::TensorDesc desc;
    desc.setPrecision(InferenceEngine::Precision::FP32);
    desc.setDims({1, 10});
    auto blob = InferenceEngine::make_shared_blob<float>(desc);
    blob->allocate();
    ireq.SetBlob("b", blob);
    auto res =  ireq.GetBlob("b");
    

    return 0;
}
