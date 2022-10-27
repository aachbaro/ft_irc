#include "../inc/Server.hpp"

void Server::set_or_change_nick(const std::string new_nickname, std::list<Client>::iterator itclient) {

    if (new_nickname.length() <= 9)
    {
        const char  *cpy = new_nickname.c_str();
        std::string allowed_char("ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefhijklmnopqrstuvwxyz0123456789`|^_-{}[]");
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
            itclient->set_nick(new_nickname);
        else
        {
            std::string msg = generate_reply("433", itclient->get_nick(), new_nickname);
            send_to_client(*itclient, msg);
        }
    }
}