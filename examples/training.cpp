#include <micrograd/nn.hpp>

using namespace nn;

#define SIZE 3
#define BATCH 4
/* #define BATCH 4 */
#define DATASET_SIZE 4

typedef double TYPE;

int main() {
    // Binary classification

    // define the neural network
    std::array<size_t, SIZE> n_neurons_for_layer = {4, 4, 1};
    auto model = MLP<TYPE, SIZE>(3, n_neurons_for_layer);

    std::vector<Value_Vec<TYPE>> xs = {
        {2.0, 3.0, -1.0}, {3.0, -1.0, 0.5}, {0.5, 1.0, 1.0}, {1.0, 1.0, -1.0}};

    // desired target
    Value_Vec<TYPE> ys = {1.0, -1.0, -1.0, 1.0};

    std::cout << model; // to output the network shape

    std::cout << "\nThe network has: " << model.parameters().size()
              << " parameters\n\n";

    double lr = 0.5;

    std::cout << "Starting Training\n";
    std::cout << "----------------------------\n\n";

    for (size_t j = 1; j <= 1000; j++) {

        std::vector<std::vector<Value_Vec<TYPE>>> ypred;
        // Create a tmp variable that allows the full graph to be stored
        Value_Vec<TYPE> tmp_loss;

        Value<TYPE> loss = Value<TYPE>(0.0, "loss");
        // Problem is with the ^ operator

        // Zero grad
        model.zero_grad();

        for (size_t i = 0; i < BATCH; i++) {
            // Forward pass - target
            ypred.emplace_back(model(xs[i]));

            // Mean Squared Error tmp
            tmp_loss.emplace_back(ypred[i][SIZE][0] - ys[i]);
        }

        // I have to compute this outside to allow the gradient to propagate
        // correctly
        for (size_t i = 0; i < BATCH; i++) {
            loss += tmp_loss[i] ^ 2.0;
        }

        // backward pass
        loss.backward();

        // Change the learning rate
        if (j < 500) {
            lr = 0.5;
        } else {
            lr = 0.1;
        }

        // Update parameters thanks to the gradient
        for (Value<TYPE>* p : model.parameters()) {
            // Update parameter value
            /* p->data += -lr * (p->grad); */
            p->data += -0.01 * (p->grad);
        }

        for (Value<TYPE>* p : model.parameters()) {
            std::cout << *p << '\n';
        }

        std::cout << "The loss at step: " << j << " is: " << loss.data << '\n';
        if (j == 2){
            /* loss.draw_graph(); */
            break;
        }
    }
}
