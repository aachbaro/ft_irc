#include "../inc/Server.hpp"

void Server::topic(std::string chan, std::string topic, std::list<Client>::iterator itclient) {
    /* CHECK IF THE CHAN EXISTS */
    std::list<Channel>::iterator itChan = _channels.begin();
    std::list<Channel>::iterator itChanend = _channels.end();
    bool    isExisting(0);

    while (itChan != itChanend) {
        if (itChan->get_name() == chan) { isExisting = 1; break ; }
        itChan++;
    }
    if (!isExisting) {
        std::string reply = generate_reply("403", itclient->get_nick(), "No such channel");
        send_to_client(*itclient, reply);
        return ;
    }
    /* CHECK IF THE ASKER IS IN THE CHAN */
    std::list<Client> chan_clients = itChan->get_clients();
    std::list<Client>::iterator itUsers = chan_clients.begin();
    std::list<Client>::iterator itUsersend = chan_clients.end();
    bool    isInChan(0);

    while (itUsers != itUsersend) {
        if (itclient->get_nick() == itUsers->get_nick()) { isInChan = 1; break ; };
        itUsers++;
    }
    if (!isInChan) {
        std::string reply = generate_reply("442", itclient->get_nick(), "You're not on that channel");
        send_to_client(*itclient, reply);
        return ;
    }
    if (itChan->getProtecTopic()) {
        if (itclient->get_nick() != itChan->get_chanOp().get_nick()) {
            std::string reply = generate_reply("482", itclient->get_nick(), "You're not channel operator");
            send_to_client(*itclient, reply);
            return ;
        }
    }
    topic = topic.substr(topic.find(":") + 1);
    itChan->setTopic(topic);
    //send_to_client();
}

void    Server::topic(std::string chan, std::list<Client>::iterator itclient)
{
    /* CHECK IF THE CHAN EXISTS */
    std::list<Channel>::iterator itChan = _channels.begin();
    std::list<Channel>::iterator itChanend = _channels.end();
    bool    isExisting(0);

    while (itChan != itChanend) {
        if (itChan->get_name() == chan) { isExisting = 1; break ; }
        itChan++;
    }
    if (!isExisting) {
        std::string reply = generate_reply("403", itclient->get_nick(), "No such channel");
        send_to_client(*itclient, reply);
        return ;
    }

    /* CHECK IF THE ASKER IS IN THE CHAN */
    std::list<Client> chan_clients = itChan->get_clients();
    std::list<Client>::iterator itUsers = chan_clients.begin();
    std::list<Client>::iterator itUsersend = chan_clients.end();
    bool    isInChan(0);

    while (itUsers != itUsersend) {
        if (itUsers->get_nick() == itclient->get_nick()) { isInChan = 1; break ; };
        itUsers++;
    }
    if (!isInChan) {
        std::string reply = generate_reply("442", itclient->get_nick(), "You're not on that channel");
        send_to_client(*itclient, reply);
        return ;
    }
    if (itChan->get_topic().empty()) { itChan->send(generate_reply("331", itclient->get_nick(), ""), *itclient, true); }
    else {
        std::string msg_topic = ":127.0.0.1 332 " + itclient->get_nick() + " " + itChan->get_name() + " " + itChan->get_topic() + "\r\n";
        itChan->send(msg_topic, *itclient, true);
        std::string timer(ctime(&start));
        msg_topic = ":127.0.0.1 333 " + itclient->get_nick() + " " + itChan->get_name() + " :" + timer + "\r\n";
        itChan->send(msg_topic, *itclient, true);
    }
}