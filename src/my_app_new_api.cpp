#include <iostream>
#include <openvino/openvino.hpp>


int main() {
    int elem_num = 51;

    ov::runtime::Core ie;
    std::shared_ptr<ov::Function> model = ie.read_model("src/test/dummy/1/dummy.xml");

    ov::PartialShape input_shape{1, ngraph::Dimension(1, 50)};
    std::map<std::string, ov::PartialShape> new_shapes{{"b", input_shape}};
    model->reshape(new_shapes);

    ov::runtime::ExecutableNetwork exec_network = ie.compile_model(model, "CPU");
    ov::runtime::InferRequest infer_request = exec_network.create_infer_request();
    ov::runtime::Tensor tensor(ov::element::f32, {1, elem_num});
    float* data = tensor.data<float>();

    for (int i = 0; i < elem_num; i++) {
        data[i] = i + 1.2f;
        std::cout << data[i] << ",";
    }
    std::cout << std::endl;

    infer_request.set_input_tensor(tensor);
    infer_request.infer();

    ov::runtime::Tensor output = infer_request.get_output_tensor();

    float* output_data = output.data<float>();
    for (int i = 0; i < elem_num; i++) {
        std::cout << output_data[i] << ",";
    }
    std::cout << std::endl;

    return 0;
}
