#ifndef CHANNEL_HPP
#define CHANNEL_HPP

# include <string>
# include <vector>
# include "Client.hpp"

class Channel {
    /* Represent Channel active in the Server. */
    public:
        Channel();
        Channel(std::string name);
        Channel(std::string name, Client client);
        ~Channel();

        Channel & operator=( Channel const & rhs );

        std::string get_name();
        std::list<Client> get_clients();
        bool                getMode();
        bool                getProtecTopic();

        int add_client(Client client);
        void    set_operator(Client client);
        void send(std::string msg, Client client, bool send_to_same);
        void names(Client client);
        std::string get_topic();
        Client get_chanOp();
        void    addInvited(std::string invited);
        bool    isInInvited(std::string nick);
        void    setMode(bool mode);
        void    setProtecTopic(bool mode);
        void    setTopic(std::string newtopic);
        void        leave_channel(Client client, std::string reason, std::string address);
        std::list<Client>::iterator find_client(std::string nick);
        void    del_client_by_nick(std::string nick_name);
        void    change_clients_nick(std::string old_nick, std::string new_nick);
        bool    actualize(void);

    private:
        std::string _name;
        std::list<Client> _clients;
        Client              _chanOperator;
        std::vector<std::string> _invited;
        std::string _topic;
        bool        _inviteOnly;
        bool        _protectedTopic;

};

#endif
