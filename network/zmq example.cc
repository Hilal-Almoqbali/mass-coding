#include <zmq.hpp>
#include <string>
#include <iostream>

int main()
{
  // Create a ZeroMQ context and socket
  zmq::context_t context(1);
  zmq::socket_t socket(context, ZMQ_REQ);

  // Get the IP address to connect to from the user
  std::string ip_address;
  std::cout << "Enter the IP address to connect to: ";
  std::getline(std::cin, ip_address);

  // Connect the socket to the IP address
  std::string connection_string = "tcp://" + ip_address + ":5555";
  socket.connect(connection_string);

  while (true)
  {
    // Get the message to send from the user
    std::string message;
    std::cout << "Enter a message to send: ";
    std::getline(std::cin, message);

    // Send the message to the server
    zmq::message_t request(message.size());
    memcpy(request.data(), message.data(), message.size());
    socket.send(request);

    // Receive a response from the server
    zmq::message_t reply;
    socket.recv(&reply);
    std::string response(static_cast<char*>(reply.data()), reply.size());

    // Print the response to the console
    std::cout << "Received: " << response << std::endl;
  }

  return 0;
}
