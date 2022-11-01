#include "../inc/Server.hpp"

void Server::mode_cmd(std::string target, std::string param, std::list<Client>::iterator itclient) {
    std::vector<Channel>::iterator it = this->_channels.begin();
    std::vector<Channel>::iterator ite = this->_channels.end();
    for (; it != ite; ++it) {
        if (it->get_name() == target) {
            if (param == "+i") {
                if (it->get_chanOp().get_nick() == itclient->get_nick())
                {
                    it->setMode(true);
                    send_to_client(*itclient, generate_reply("324", itclient->get_nick(), it->get_name() + " channel mode is +i"));
                }
                else
                    send_to_client(*itclient, generate_reply("483", itclient->get_nick(), "You're not channel operator"));
            }
            if (param == "-i") {
                if (it->get_chanOp().get_nick() == itclient->get_nick())
                {
                    it->setMode(false);
                    send_to_client(*itclient, generate_reply("324", itclient->get_nick(), it->get_name() + " channel mode is -i"));
                }
                else
                    send_to_client(*itclient, generate_reply("483", itclient->get_nick(), "You're not channel operator"));
            }
        }
    }
}