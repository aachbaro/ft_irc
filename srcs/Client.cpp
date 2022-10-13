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

void		Client::handle_new_entry(std::string str)
{
	std::string key = str.substr(0, str.find(" "));

	if (!key.compare("PASS"))
		this->pass = str.substr(str.find(" ") + 1, str.find("\r") - str.find(" ") - 1);
	else if (!key.compare("NICK"))
		this->nick = str.substr(str.find(" ") + 1, str.find("\r") - str.find(" ") - 1);
	else if (!key.compare("USER"))
	{
		this->user = str.substr(str.find(" ") + 1, (str.find(" ", str.find(" ") + 1)) - str.find(" ") - 1);
		str = str.substr(str.find(" ", str.find(" ") + 1));
		this->mode = str.substr(str.find(" ") + 1, (str.find(" ", str.find(" ") + 1)) - str.find(" ") - 1);
		str = str.substr(str.find(" ", str.find(" ") + 1));
		this->unused = str.substr(str.find(" ") + 1, (str.find(" ", str.find(" ") + 1)) - str.find(" ") - 1);
		str = str.substr(str.find(" ", str.find(" ") + 1));
		this->realname = str.substr(str.find(":") + 1);
	}
}

void	Client::connection()
{
	while (this->nick.empty() || pass.empty() || user.empty())
	{
		if (this->save.empty())
			recv(fd, buf, 1000, 0);
		if (!complete_command())
			handle_new_entry(this->cmd);
	}
}

int		Client::complete_command()
{
	std::string cpy(this->buf);

	this->save += cpy;
	if (this->save.find("\r") != -1)
	{
		this->cmd = this->save.substr(0, this->save.find("\r"));
		this->save = this->save.substr(this->save.find("\n") + 1);
		return (0); 
	}
	else
		return (1);
}

/*
** --------------------------------- ACCESSOR ---------------------------------
*/

std::string		Client::get_nick() {return (this->nick); }
std::string		Client::get_pass() {return (this->pass); }
std::string		Client::get_user() {return (this->user); }
std::string		Client::get_mode() {return (this->mode); }
std::string		Client::get_unused() {return (this->unused); }
std::string		Client::get_realname() {return (this->realname); }
char			*Client::get_buf() {return (this->buf); }
int				Client::get_fd() {return (this->fd); }

void			Client::set_nick(std::string nickname) {this->nick = nickname; }


/* ************************************************************************** */