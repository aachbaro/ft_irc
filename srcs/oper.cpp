#include "../inc/Server.hpp"

void Server::oper(Client client, std::string name, std::string password) {
    if (name != "admin" || password != "oper_password") {
        std::string reply = generate_reply("464", client.get_nick(), "Password incorrect");
        send_to_client(client, reply);
        return ;
    }
    if (client.get_host() != "127.0.0.1") {
        std::string reply = generate_reply("491", client.get_nick(), "No O-lines for your host");
        send_to_client(client, reply);
        return ;
    }
    client.set_operator(true);
    std::string reply = generate_reply("381", client.get_nick(), "You are now an IRC operator");
    send_to_client(client, reply);
}
