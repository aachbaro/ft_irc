#include "../inc/Server.hpp"

void Server::invite(std::string channel, std::string target, std::list<Client>::iterator itclient) {

    /* CHECK IF THE CHAN EXISTS */
    std::list<Channel>::iterator itChan = _channels.begin();
    std::list<Channel>::iterator itChanend = _channels.end();
    bool    isExisting(0);

    while (itChan != itChanend) {
        if (itChan->get_name() == channel) { isExisting = 1; break ; }
        itChan++;
    }
    if (!isExisting) {
        std::string reply = generate_reply("403", itclient->get_nick(), "No such channel");
        send_to_client(*itclient, reply);
        return ;
    }

    /* CHECK IF THE ASKER IS IN THE CHAN */
    std::list<Client> cpyClient = itChan->get_clients();
    std::list<Client>::iterator itUsers = cpyClient.begin();
    std::list<Client>::iterator itUsersend = cpyClient.end();
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
    if (itChan->getMode()) {
        if (itclient->get_nick() != itChan->get_chanOp().get_nick()) {
            std::string reply = generate_reply("482", itclient->get_nick(), "You're not channel operator");
            send_to_client(*itclient, reply);
            return ;
        }
    }
    /* CHECK IF THE INVITED IS IN THE CHAN */
    itUsers = cpyClient.begin();
    isInChan = 0;

    while (itUsers != itUsersend) {
        if (itUsers->get_nick() == target) { isInChan = 1; break ; };
        itUsers++;
    }
    if (isInChan) {
        std::string reply = generate_reply("443", itclient->get_nick(), target + "is already on channel");
        send_to_client(*itclient, reply);
        return ;
    }
    /* SEND INVITATION TO THE TARGET */
    if (find_client_by_nick(target) == clients.end()) { return ;}
    std::string reply = ":" + address + " 341 " + itclient->get_nick() + " " + target + " " + channel + "\r\n";
    send_to_client(*itclient, reply);
    reply = ":" + itclient->get_nick() + " INVITE " + target + " " + itChan->get_name() + "\r\n";
    send_to_client(*(find_client_by_nick(target)), reply);
    itChan->addInvited(target);
}