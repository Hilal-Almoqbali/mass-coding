#version 330 core

// Activation function
float sigmoid(float x)
{
    return 1.0 / (1.0 + exp(-x));
}

// Derivative of the activation function
float sigmoidDerivative(float x)
{
    return x * (1.0 - x);
}

// Neural network structure
struct NeuralNetwork {
    float biases[3]; // Neural network biases (input, hidden, output)
    float weights[3][3]; // Neural network weights (input-hidden, hidden-output)
};

// Initialize neural network
void initNetwork(out NeuralNetwork network)
{
    network.biases[0] = (float)rand() / RAND_MAX; // Random bias between 0 and 1
    network.biases[1] = (float)rand() / RAND_MAX; // Random bias between 0 and 1
    network.biases[2] = (float)rand() / RAND_MAX; // Random bias between 0 and 1

    for (int i = 0; i < 3; i++) {
        for (int j = 0; j < 3; j++) {
            network.weights[i][j] = (float)rand() / RAND_MAX; // Random weight between 0 and 1
        }
    }
}

// Forward pass through the network
vec3 forwardPass(NeuralNetwork network, vec3 input)
{
    vec3 hidden = sigmoid(input * network.weights[0] + network.biases[0]);
    vec3 output = sigmoid(hidden * network.weights[1] + network.biases[1]);
    return output;
}

// Backward pass through the network
void backwardPass(inout NeuralNetwork network, vec3 expectedOutput, vec3 input)
{
    // Calculate errors for output layer
    vec3 outputError = expectedOutput - output;
    vec3 outputGradient = outputError * sigmoidDerivative(output);

    // Calculate errors for hidden layer
    vec3 hiddenError = outputGradient * network.weights[1];
    vec3 hiddenGradient = hiddenError * sigmoidDerivative(hidden);

    // Update weights and biases
    network.weights[1] += hidden * outputGradient * LEARNING_RATE;
    network.weights[0] += input * hiddenGradient * LEARNING_RATE;
    network.biases[1] += outputGradient * LEARNING_RATE;
    network.biases[0] += hiddenGradient * LEARNING_RATE;
}

int main()
{
    // Seed random number generator
    srand(time(nullptr));

    // Create neural network
    NeuralNetwork network;
    initNetwork(network);

    // Training data
    vec3 trainingData[4] = {
        vec3(0, 0, 1),
        vec3(0, 1, 0),
        vec3(1, 0, 0),
        vec3(1, 1, 0)
    };

    // Train network
    for (int epoch = 0; epoch < 1000; epoch++) {
        // Loop through all training examples
        for (int i = 0; i < 4; i++) {
            vec3 input = trainingData[i].xy;
            vec3 expectedOutput = vec3(trainingData[i].z);

            // Forward pass
            vec3 actualOutput = forwardPass(network, input);

            // Backward pass
            backwardPass(network, expectedOutput, input);
        }
    }

    // Test network
    vec3 testInput = vec3(0, 1);
    vec3 testOutput = forwardPass(network, testInput);
    std::cout << "Output: " << testOutput.x << std::endl;

    return 0;
}
