#ifndef CLIENT_HPP
# define CLIENT_HPP

# include <iostream>
# include <string>
# include <string.h>

class Client
{

	public:

		Client(int fd);
		Client( Client const & src );
		~Client();

		Client &		operator=( Client const & rhs );

	std::string			handle_new_entry(std::string str);

	std::string			get_cap();
	std::string			get_nick();
	std::string			get_pass();
	int					get_fd();

	private:

	std::string			nick;
	std::string			fullname;
	std::string			pass;
	std::string			cap;
	std::string			server;
	std::string			username;
	int					fd;


};

std::ostream &			operator<<( std::ostream & o, Client const & i );

#endif /* ********************************************************** CLIENT_H */