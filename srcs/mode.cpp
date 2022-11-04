#include "../inc/Server.hpp"

void Server::mode_cmd(std::string target, std::string param, std::list<Client>::iterator itclient) {
    std::vector<Channel>::iterator it = this->_channels.begin();
    std::vector<Channel>::iterator ite = this->_channels.end();
    const char    *cpy = param.c_str();
    int             i(0);
    bool           plusminus(0);
    for (; it != ite; ++it) {
        if (it->get_name() == target) {
            if (it->get_chanOp().get_nick() == itclient->get_nick()) {
                while (cpy[i]) {
                    if (cpy[i] == '+') { plusminus = 1; }
                    else if (cpy[i] == '-') { plusminus = 0; }
                    else if (cpy[i] == 'i') { it->setMode(plusminus); }
                    else if (cpy[i] == 't') { it->setMode(plusminus); }
                    i++;
                }
                send_to_client(*itclient, generate_reply("324", itclient->get_nick(), it->get_name() + " channel mode is " + param));
            }
            else { send_to_client(*itclient, generate_reply("483", itclient->get_nick(), "You're not channel operator")); }

/*            if (param == "+i") {
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
            }*/
        }
    }
}