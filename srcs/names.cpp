#include "../inc/Server.hpp"

void Server::names(Client client, std::string name) {
    std::list<Channel>::iterator it = this->_channels.begin();
    std::list<Channel>::iterator ite = this->_channels.end();
    for (; it != ite; ++it) {
        if (it->get_name() == name) {
            it->names(client);
            return ;
        }
    }
    std::string msg = ":127.0.0.1 366 " + client.get_nick() + " " + name + " :End of NAMES list\r\n";
    Server::send_to_client(client, msg);
}
