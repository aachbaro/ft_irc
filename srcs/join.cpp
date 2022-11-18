#include "../inc/Server.hpp"

std::vector<std::string> Server::get_join_args(std::string parsed) {
    std::vector<std::string> channels;
    size_t pos = 0;
    while((pos = parsed.find(",")) != std::string::npos) {
        std::string word = parsed.substr(0, pos);
        channels.push_back(word);
        parsed.erase(0, pos + 1);
    }
    if (parsed.length() > 0)
        channels.push_back(parsed);
    return channels;
}

void Server::join_or_create_channel(std::string name, std::list<Client>::iterator itclient) {
    /*
        Handle JOIN command.
        - Check if Channel already exists, if so, just add the new client to the channel.
        - If channel doesn't exist, create it.
        - For both cases, send message to client
    */
    std::list<Channel>::iterator it = this->_channels.begin();
    std::list<Channel>::iterator ite = this->_channels.end();
    for (; it != ite; ++it) {
        if (it->get_name() == name) {
            if (it->getMode() == true && !it->isInInvited(itclient->get_nick())) {
                send_to_client(*itclient, generate_reply("473", itclient->get_nick(), it->get_name()));
                return ;
            }
            it->add_client(*itclient);
            std::string msg = ":" + itclient->get_nick() + " JOIN " + name + "\r\n";
            it->send(msg, *itclient, true);
            if (it->get_topic().length() > 0) {
                std::string msg_topic = ":127.0.0.1 332 " + itclient->get_nick() + " " + it->get_name() + " :" + it->get_topic() + "\r\n";
                send_to_client(*itclient, msg_topic);
            }
            it->names(*itclient);
            return ;
        }
    }
    Channel new_channel(name, *itclient);
    _channels.push_back(new_channel);
    std::string msg = ":" + itclient->get_nick() + " JOIN " + name + "\r\n";
    new_channel.send(msg, *itclient, true);
    new_channel.names(*itclient);
}
