#include "../inc/Server.hpp"

std::string Server::generate_reply(std::string code, std::string target, std::string msg) {
    /* Generate string message (Former reply function) - Not used anywhere for now */
    return (":" + this->address + " " + code + " " + target + " :" + msg + "\r\n");
}
