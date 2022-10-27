#include "../inc/Server.hpp"

void Server::join_or_create_channel(std::string name, std::string topic, std::list<Client>::iterator itclient) {
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
            std::string msg = ":" + itclient->get_nick() + " JOIN " + name + "\r\n";
            it->send(msg, *itclient);
            it->names(*itclient);
            return ;
        }
    }
    Channel new_channel(name, *itclient, topic);
    _channels.push_back(new_channel);
    std::string msg = ":" + itclient->get_nick() + " JOIN " + name + "\r\n";
    new_channel.send(msg, *itclient);
    new_channel.names(*itclient);
    std::string msg_topic = ":127.0.0.1 332 " + itclient->get_nick() + " " + new_channel.get_name() + " :" + topic;
    std::cout << "MSG TOPIC: " << msg_topic << std::endl;
    send_to_client(*itclient, msg_topic);
}
