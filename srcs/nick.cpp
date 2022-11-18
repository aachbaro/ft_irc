#include "../inc/Server.hpp"

void Server::set_or_change_nick(const std::string new_nickname, std::list<Client>::iterator itclient) {
    if (itclient->get_pass() == "") {return ;}
    if (new_nickname == itclient->get_nick())
        return ;
    if (new_nickname.length() <= 20)
    {
        const char  *cpy = new_nickname.c_str();
        std::string allowed_char("ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefhijklmnopqrstuvwxyz0123456789`|^_-{}[]\\");
        int        i(0);

        while (cpy[i])
        {
            if (allowed_char.find(cpy[i]) == -1)
            {
                std::string msg = generate_reply("432", itclient->get_nick(), "Erroneous nickname");
                send_to_client(*itclient, msg);
                return ;
            }
            i++;
        }
        if (!check_nicknames(new_nickname))
        {
            std::string name;
            if (itclient->get_nick().empty()) { name = new_nickname; }
            else { name = itclient->get_nick(); }
            std::string msg = ":" + name + " NICK " + new_nickname + "\r\n";
            change_nick_in_channels(itclient->get_nick(), new_nickname);
            itclient->set_nick(new_nickname);
            if (!itclient->get_nick().empty())
                send_to_client(*itclient, msg);
        }
        else
        {
            std::string msg = generate_reply("433", itclient->get_nick(), new_nickname);
            send_to_client(*itclient, ":" + address + " 433 " + itclient->get_nick() + " " + new_nickname + " :Nickname is already in use\r\n");
        }
    }
    else
    {
        std::string msg = generate_reply("", itclient->get_nick(), new_nickname + " Nickname too long, max. 20 characters");
        send_to_client(*itclient, msg);
    }
}

void    Server::change_nick_in_channels(const std::string old_nick, const std::string new_nick)
{
    std::list<Channel>::iterator  itChan = _channels.begin();
    std::list<Channel>::iterator  itChanend = _channels.end();

    while (itChan != itChanend)
    {
        itChan->change_clients_nick(old_nick, new_nick);
        itChan++;
    }
}