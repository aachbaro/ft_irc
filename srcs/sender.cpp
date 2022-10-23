#include "../inc/Server.hpp"

void Server::send_welcome_msg(Client client) {
    /* First welcome message to send to client when it connects to the server. */
    std::string str_welcome = "Welcome to the Internet Relay Chat Network" + client.get_nick() + " !\n";
    std::string str_host = "Your host " + this->address + " is runnning version 0.0.\n";
    std::string str_date = "This sever has been started 20/10/2022 at 00:00:00.";
    std::string reply = this->reply("001", client.get_nick(), str_welcome + str_host + str_date);
    send(client.get_fd(), reply.c_str(), reply.length() + 1, SOCK_STREAM);
}
