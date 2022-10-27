#include "../inc/Server.hpp"

void  Server::names(Client client) {
    std::vector<Channel>::iterator it = _channels.begin();
    std::vector<Channel>::iterator ite = _channels.end();
    for (; it != ite; ++it) {
        it->names(client);
    }
}