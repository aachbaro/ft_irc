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
        std::vector<Client> get_clients();

        int add_client(Client client);

    private:
        std::string _name;
        std::vector<Client> _clients;
};

#endif
