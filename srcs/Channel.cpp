#include "../inc/Channel.hpp"

Channel::Channel() {}
Channel::Channel(std::string name): _name(name), _clients() {}
Channel::Channel(std::string name, Client client): _name(name) {
    /*
        Constructor with client
        - Add client in Channel
    */
    add_client(client);
}
Channel::~Channel() {
    this->_clients.clear();
}

Channel & Channel::operator=(Channel const & rhs) {
    this->_name = rhs._name;;
    this->_clients = rhs._clients;

    return *this;
}

std::string Channel::get_name() {
    return this->_name;
}

std::vector<Client> Channel::get_clients() {
    return this->_clients;
}

int Channel::add_client(Client client) {
    /*
        Add client to the channel
        - If client already in channel, return 0
        - If client added, return 1;
    */
    std::vector<Client>::iterator it = _clients.begin();
    std::vector<Client>::iterator ite = _clients.end();

    for (; it != ite; ++it) {
        if (it->get_nick() == client.get_nick())
            return 0;
    }

    this->_clients.push_back(client);
    return 1;
}
