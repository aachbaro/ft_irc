#ifndef CLIENT_HPP
# define CLIENT_HPP

# include <iostream>
# include <string.h>
# include <sys/types.h>
# include <sys/socket.h>
# include <netdb.h>
# include <poll.h>
# include <iostream>
# include <string>
# include <cstring>
# include <list>
# include <unistd.h>
# include <fcntl.h>

class Client
{

	public:

		Client(int fd);
		Client( Client const & src );
		~Client();

		Client &		operator=( Client const & rhs );

	void				handle_new_entry(std::string str);
	int					complete_command();
	void				connection();
	void				get_fullcmd();

	std::string			get_cap();
	std::string			get_nick();
	std::string			get_pass();
	std::string			get_user();
	std::string			get_mode();
	std::string			get_unused();
	std::string			get_realname();
	char				*get_buf();
	int					get_fd();
	std::string			get_cmd();

	void				set_nick(const std::string nickname);

	private:

	std::string			nick;
	std::string			user;
	std::string			mode;
	std::string			unused;
	std::string			realname;
	std::string			pass;
	int					fd;
	char				buf[1000];
	std::string			save;
	std::string			cmd;

};

std::ostream &			operator<<( std::ostream & o, Client const & i );

#endif /* ********************************************************** CLIENT_H */