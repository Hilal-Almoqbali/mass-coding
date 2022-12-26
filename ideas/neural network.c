#include <stdio.h>
#include <stdlib.h>
#include <time.h>

#define INPUT_SIZE 2 // Number of input neurons
#define HIDDEN_SIZE 3 // Number of hidden neurons
#define OUTPUT_SIZE 1 // Number of output neurons
#define LEARNING_RATE 0.1f // Learning rate for gradient descent

// Activation function
float sigmoid(float x)
{
    return 1.0f / (1.0f + exp(-x));
}

// Derivative of the activation function
float sigmoidDerivative(float x)
{
    return x * (1.0f - x);
}

// Neuron structure
struct Neuron {
    float* weights; // Neuron weights
    float bias; // Neuron bias
    float value; // Neuron value
    float error; // Neuron error
};

// Neural network structure
struct NeuralNetwork {
    struct Neuron** layers; // Neural network layers (input, hidden, output)
};

// Initialize neural network
void initNetwork(struct NeuralNetwork* network)
{
    // Allocate memory for layers
    network->layers = malloc(3 * sizeof(struct Neuron*));

    // Allocate memory for input layer
    network->layers[0] = malloc(INPUT_SIZE * sizeof(struct Neuron));
    for (int i = 0; i < INPUT_SIZE; i++) {
        network->layers[0][i].bias = (float)rand() / RAND_MAX; // Random bias between 0 and 1
    }

    // Allocate memory for hidden layer
    network->layers[1] = malloc(HIDDEN_SIZE * sizeof(struct Neuron));
    for (int i = 0; i < HIDDEN_SIZE; i++) {
        network->layers[1][i].bias = (float)rand() / RAND_MAX; // Random bias between 0 and 1
        network->layers[1][i].weights = malloc(INPUT_SIZE * sizeof(float));
        for (int j = 0; j < INPUT_SIZE; j++) {
            network->layers[1][i].weights[j] = (float)rand() / RAND_MAX; // Random weight between 0 and 1
        }
    }

    // Allocate memory for output layer
    network->layers[2] = malloc(OUTPUT_SIZE * sizeof(struct Neuron));
    for (int i = 0; i < OUTPUT_SIZE; i++) {
        network->layers[2][i].bias = (float)rand() / RAND_MAX; // Random bias between 0 and 1
        network->layers[2][i].weights = malloc(HIDDEN_SIZE * sizeof(float));
        for (int j = 0; j < HIDDEN_SIZE; j++) {
            network->layers[2][i].weights[j] = (float)rand() / RAND_MAX; // Random weight between 0 and 1
        }
    }
}


// Forward pass through the network
void forwardPass(struct NeuralNetwork* network, float* input, float* output)
{
    // Set input layer values
    for (int i = 0; i < INPUT_SIZE; i++) {
        network->layers[0][i].value = input[i];
    }

    // Calculate values for hidden layer
    for (int i = 0; i < HIDDEN_SIZE; i++) {
        float sum = network->layers[1][i].bias;
        for (int j = 0; j < INPUT_SIZE; j++) {
            sum += network->layers[1][i].weights[j] * network->layers[0][j].value;
        }
        network->layers[1][i].value = sigmoid(sum);
    }

    // Calculate values for output layer
    for (int i = 0; i < OUTPUT_SIZE; i++) {
        float sum = network->layers[2][i].bias;
        for (int j = 0; j < HIDDEN_SIZE; j++) {
            sum += network->layers[2][i].weights[j] * network->layers[1][j].value;
        }
        output[i] = sigmoid(sum);
    }
}

// Backward pass through the network
void backwardPass(struct NeuralNetwork* network, float* expectedOutput)
{
    // Calculate errors for output layer
    for (int i = 0; i < OUTPUT_SIZE; i++) {
        network->layers[2][i].error = expectedOutput[i] - network->layers[2][i].value;
        network->layers[2][i].error *= sigmoidDerivative(network->layers[2][i].value);
    }

    // Calculate errors for hidden layer
    for (int i = 0; i < HIDDEN_SIZE; i++) {
        float errorSum = 0.0f;
        for (int j = 0; j < OUTPUT_SIZE; j++) {
            errorSum += network->layers[2][j].error * network->layers[2][j].weights[i];
        }
        network->layers[1][i].error = errorSum * sigmoidDerivative(network->layers[1][i].value);
    }
}

// Update weights and biases
void updateWeights(struct NeuralNetwork* network, float* input)
{
    // Update weights and biases for output layer
    for (int i = 0; i < OUTPUT_SIZE; i++) {
        for (int j = 0; j < HIDDEN_SIZE; j++) {
            network->layers[2][i].weights[j] += LEARNING_RATE * network->layers[1][j].value * network->layers[2][i].error;
        }
        network->layers[2][i].bias += LEARNING_RATE * network->layers[2][i].error;
    }

    // Update weights and biases for hidden layer
    for (int i = 0; i < HIDDEN_SIZE; i++) {
        for (int j = 0; j < INPUT_SIZE; j++) {
            network->layers[1][i].weights[j] += LEARNING_RATE * input[j] * network->layers[1][i].error;
        }
        network->layers[1][i].bias += LEARNING_RATE * network->layers[1][i].error;
    }
}

int main()
{
    // Seed random number generator
    srand(time(NULL));

    // Create neural network
    struct NeuralNetwork network;
    initNetwork(&network);

    // Training data
    float trainingData[4][3] = {
        {0, 0, 1},
        {0, 1, 0},
        {1, 0, 0},
        {1, 1, 0}
    };

    // Train network
    for (int epoch = 0; epoch < 1000; epoch++) {
        // Loop through all training examples
        for (int i = 0; i < 4; i++) {
            float input[2] = {trainingData[i][0], trainingData[i][1]};
            float expectedOutput[1] = {trainingData[i][2]};

            // Forward pass
            float output[1];
            forwardPass(&network, input, output);

            // Backward pass
            backwardPass(&network, expectedOutput);

            // Update weights and biases
            updateWeights(&network, input);
        }
    }

    // Test network
    float testInput[2] = {0, 1};
    float testOutput[1];
    forwardPass(&network, testInput, testOutput);
    printf("Output: %f\n", testOutput[0]);

    return 0;
}
