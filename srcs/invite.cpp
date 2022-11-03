#include "../inc/Server.hpp"

void Server::invite(std::string channel, std::string target, std::list<Client>::iterator itclient) {

    /* CHECK IF THE CHAN EXISTS */
    std::vector<Channel>::iterator itChan = _channels.begin();
    std::vector<Channel>::iterator itChanend = _channels.end();
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
    std::vector<Client>::iterator itUsers = itChan->get_clients().begin();
    std::vector<Client>::iterator itUsersend = itChan->get_clients().end();
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
    itUsers = itChan->get_clients().begin();
    itChan->get_clients().end();
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
    std::string reply = generate_reply("341", find_client_by_nick(target).get_nick(), channel);
    send_to_client(find_client_by_nick(target), reply);
    itChan->addInvited(target);
}