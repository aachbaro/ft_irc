#include "../inc/Server.hpp"

void Server::pong_reply(std::string address, Client client) {
    std::string msg = ":" + address + " PONG " + this->address + " :" + this->address + "\r\n";
    send_to_client(client, msg);
}
