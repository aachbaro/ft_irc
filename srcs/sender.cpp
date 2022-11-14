#include "../inc/Server.hpp"

void Server::send_to_client(Client client, std::string msg) {
    send(client.get_fd(), msg.c_str(), msg.size(), MSG_DONTWAIT);
}

void Server::send_welcome_msg(Client client) {
    /* First welcome message to send to client when it connects to the server. */
    std::string str_welcome = ":" + this->address +  " 001 " + client.get_nick() +  " :Welcome to the Internet Relay Chat Network " + client.get_nick() +  "! " + client.get_user() + "@" + client.get_host() +  " \r\n";
    send_to_client(client, str_welcome);
    std::string str_host = ":" + this->address +  " 002 " + client.get_nick() + " :Your host is " + this->address + ", runnning version 0.0.\r\n";
    send_to_client(client, str_host);
    std::string str_date = ":" + this->address +  " 003 " + client.get_nick() + " :This sever has been started 20/10/2022 at 00:00:00." + "\r\n";
    send_to_client(client, str_date);
}
