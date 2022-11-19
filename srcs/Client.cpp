#include "../inc/Client.hpp"

/*
** ------------------------------- CONSTRUCTOR --------------------------------
*/


Client::Client() {
	nick = "";
	user = "";
	mode = "";
	unused = "";
	realname = "";
	pass = "";
	save = "";
	cmd = "";
	_emptyPassWord = 0;
	_operator = false;
	_connected = false;
}

Client::Client(int fd_, std::string host) : host(host)
{
	nick = "";
	user = "";
	mode = "";
	unused = "";
	realname = "";
	pass = "";
	save = "";
	cmd = "";
	_emptyPassWord = 0;
	_operator = false;
	_connected = false;
	fd = fd_;
}

Client::Client( const Client & src )
{
	nick = src.nick;
	user = src.user;
	host = src.host;
	mode = src.mode;
	unused = src.unused;
	realname = src.realname;
	pass = src.pass;
	fd = src.fd;
	save = src.save;
	cmd = src.cmd;
	_operator = src._operator;
	_connected = src._connected;
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
	if ( this != &rhs )
	{
		nick = rhs.nick;
		user = rhs.user;
		host = rhs.host;
		mode = rhs.mode;
		unused = rhs.unused;
		realname = rhs.realname;
		pass = rhs.pass;
		fd = rhs.fd;
		save = rhs.save;
		cmd = rhs.cmd;
		_operator = rhs._operator;
		_connected = rhs._connected;
	}
	return *this;
}

/*
** --------------------------------- METHODS ----------------------------------
*/

int		Client::complete_command()
{
	std::string cpy(this->buf);
	memset(buf, 0, 1000);

	this->save += cpy;
	if (this->save.find("\r") != std::string::npos)
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
std::string		Client::get_cmd() {return (this->cmd); }
std::string		Client::get_host() {return (this->host);}
bool			Client::get_operator() {return this->_operator;}
bool			Client::isRegistered() {return this->_connected;}

void			Client::set_nick(const std::string nickname) {this->nick = nickname; }
void			Client::set_username(const std::string username) {this->user = username;}
void			Client::set_operator(const bool to_op) {this->_operator = to_op;}
void			Client::set_registered(const bool connected) {this->_connected = connected;}
void			Client::set_pass(const std::string password) {this->pass = password;}
void			Client::set_realname(const std::string real_name) {this->realname = real_name;}
void			Client::clear_cmd(void) {cmd.clear(); }


/* ************************************************************************** */