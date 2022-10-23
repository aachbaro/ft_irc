#include "../inc/Server.hpp"

void Server::join_or_create_channel(std::string name, std::list<Client>::iterator itclient) {
    /*
        Handle JOIN command.
        - Check if Channel already exists, if so, just add the new client to the channel.
        - If channel doesn't exist, create it.
        - For both cases, send message to client (J'y arrive pas pour l'instant)
    */
    std::vector<Channel>::iterator it = this->_channels.begin();
    std::vector<Channel>::iterator ite = this->_channels.end();
    for (; it != ite; ++it) {
        if (it->get_name() == name) {
            it->add_client(*itclient);
            // send_to_client(*itclient, itclient->get_nick() + " has joined #" + name);
            return ;
        }
    }
    Channel new_channel(name, *itclient);
    _channels.push_back(new_channel);
    // send_to_client(*itclient, itclient->get_nick() + " has joined #" + name);
}
