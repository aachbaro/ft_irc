#include "../inc/Server.hpp"

void Server::topic(Client client, std::string channel_name, std::vector<std::string>::iterator it, std::vector<std::string>::iterator ite) {
    std::string topic = "";
    if (it != ite) {
        if ((*it)[0] != ':') {
            send_to_client(client, ":" + address + " 461 " + client.get_nick() + " TOPIC :Syntax Error\r\n");
            return ;
        }
        topic = (*it).erase(0, 1);
        ++it;
        for (; it != ite; ++it) {
            topic += " " + *it;
        }
    }
    std::list<Channel>::iterator itChan = _channels.begin();
    std::list<Channel>::iterator itChanend = _channels.end();
    for (;itChan != itChanend; ++itChan) {
        if (itChan->get_name() == channel_name) {
            break ;
        }
    }
    if (itChan == itChanend) {
        send_to_client(client, ":" + address + " 403 " + client.get_nick() + " " + channel_name + " :No such channel\r\n");
        return ;
    }
    std::list<Client> chan_clients = itChan->get_clients();
    std::list<Client>::iterator itUsers = chan_clients.begin();
    std::list<Client>::iterator itUsersend = chan_clients.end();
    for (; itUsers != itUsersend; ++itUsers) {
        if (client.get_nick() == itUsers->get_nick()) {
            break ;
        }
    }
    if (itUsers == itUsersend) {
        send_to_client(client, ":" + address + " 442 " + client.get_nick() + " :You're not on that channel\r\n");
        return ;
    }
    Client chan_op = itChan->get_chanOp();
    if (itChan->getProtecTopic()) {
        if (chan_op.get_nick() != client.get_nick()) {
            send_to_client(client, ":" + address + " 482 " + client.get_nick() + " " + channel_name + " :You're not channel operator\r\n");
            return ;
        }
    }
    itChan->setTopic(topic);
    std::string msg_topic = ":127.0.0.1 332 " + client.get_nick() + " " + itChan->get_name() + " :" + itChan->get_topic() + "\r\n";
    itChan->send(msg_topic, client, true);
}


void Server::send_topic(Client client, std::string channel_name) {
    std::list<Channel>::iterator itChan = _channels.begin();
    std::list<Channel>::iterator itChanend = _channels.end();
    for (;itChan != itChanend; ++itChan) {
        if (itChan->get_name() == channel_name) {
            break;
        }
    }
    if (itChan == itChanend) {
        send_to_client(client, ":" + address + " 403 " + client.get_nick() + " " + channel_name + " :No such channel\r\n");
        return ;
    }
    std::string chan_topic = itChan->get_topic();


    std::list<Client> chan_clients = itChan->get_clients();
    std::list<Client>::iterator itUsers = chan_clients.begin();
    std::list<Client>::iterator itUsersend = chan_clients.end();
    for (; itUsers != itUsersend; ++itUsers) {
        if (client.get_nick() == itUsers->get_nick()) {
            break ;
        }
    }
    if (itUsers == itUsersend) {
        send_to_client(client, ":" + address + " 442 " + client.get_nick() + " :You're not on that channel\r\n");
        return ;
    }
    if (chan_topic.size() == 0) {
        send_to_client(client, ":" + address + " 331 " + client.get_nick() + " :No topic is set\r\n");
        return ;
    }
    Client chan_op = itChan->get_chanOp();
    std::string msg_topic = ":127.0.0.1 332 " + client.get_nick() + " " + itChan->get_name() + " " + itChan->get_topic() + "\r\n";
    send_to_client(client, msg_topic);
    std::string timer(ctime(&start));
    msg_topic = ":127.0.0.1 333 " + client.get_nick() + " " + itChan->get_name() + " " + chan_op.get_nick() + " " + timer + "\r\n";
    send_to_client(client, msg_topic);
}
