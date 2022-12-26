#include <iostream>
#include <random>
#include <vector>

// Activation function
double sigmoid(double x) {
    return 1.0 / (1.0 + exp(-x));
}

// Derivative of activation function
double sigmoid_derivative(double x) {
    return x * (1.0 - x);
}

// Neural network class
class NeuralNetwork {
public:
    // Constructor
    NeuralNetwork(int input_size, int hidden_size, int output_size) :
        input_size(input_size),
        hidden_size(hidden_size),
        output_size(output_size),
        biases1(hidden_size),
        biases2(output_size),
        weights1(input_size, std::vector<double>(hidden_size)),
        weights2(hidden_size, std::vector<double>(output_size))
    {
        // Seed random number generator
        std::random_device rd;
        std::mt19937 gen(rd());
        std::uniform_real_distribution<> dis(0, 1);

        // Initialize weights and biases with random values between 0 and 1
        for (int i = 0; i < hidden_size; i++) {
            biases1[i] = dis(gen);
            for (int j = 0; j < input_size; j++) {
                weights1[j][i] = dis(gen);
            }
        }
        for (int i = 0; i < output_size; i++) {
            biases2[i] = dis(gen);
            for (int j = 0; j < hidden_size; j++) {
                weights2[j][i] = dis(gen);
            }
        }
    }


// Forward pass
std::vector<double> forward(const std::vector<double>& input) {
    std::vector<double> hidden(hidden_size);
    for (int i = 0; i < hidden_size; i++) {
        double sum = biases1[i];
        for (int j = 0; j < input_size; j++) {
            sum += input[j] * weights1[j][i];
        }
        hidden[i] = sigmoid(sum);
    }
    std::vector<double> output(output_size);
    for (int i = 0; i < output_size; i++) {
        double sum = biases2[i];
        for (int j = 0; j < hidden_size; j++) {
            sum += hidden[j] * weights2[j][i];
        }
        output[i] = sigmoid(sum);
    }
    return output;
}