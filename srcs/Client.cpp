#include "../inc/Client.hpp"

/*
** ------------------------------- CONSTRUCTOR --------------------------------
*/

Client::Client(int fd) : fd(fd)
{
}

Client::Client( const Client & src )
{
}


/*
** -------------------------------- DESTRUCTOR --------------------------------
*/

Client::~Client()
{
}


/*
** --------------------------------- OVERLOAD ---------------------------------
*/

Client &				Client::operator=( Client const & rhs )
{
	//if ( this != &rhs )
	//{
		//this->_value = rhs.getValue();
	//}
	return *this;
}

std::ostream &			operator<<( std::ostream & o, Client const & i )
{
	//o << "Value = " << i.getValue();
	return o;
}


/*
** --------------------------------- METHODS ----------------------------------
*/

std::string		Client::handle_new_entry(std::string str)
{
	std::string key = str.substr(0, str.find(" "));
	//std::cout << str << std::endl;
	if (!key.compare("PASS"))
		this->pass = str.substr(str.find(" ") + 1, str.find("\r") - str.find(" ") - 1);
	else if (!key.compare("NICK"))
		this->nick = str.substr(str.find(" ") + 1, str.find("\r") - str.find(" ") - 1);
	str = str.substr(str.find("\n") + 1, str.length() - str.find("\n") - 1);
	return (str);
}

/*
** --------------------------------- ACCESSOR ---------------------------------
*/

std::string		Client::get_cap() {return (this->cap); }
std::string		Client::get_nick() {return (this->nick); }
std::string		Client::get_pass() {return (this->pass); }
int				Client::get_fd() {return (this->fd); }


/* ************************************************************************** */