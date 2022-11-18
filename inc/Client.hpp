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

		Client();
		Client(int fd, std::string host);
		Client( Client const & src );
		~Client();

		Client &		operator=( Client const & rhs );

	void				handle_new_entry(std::string str);
	int					complete_command();
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
	std::string			get_host();
	bool				get_operator();
	bool				isRegistered();

	void				set_nick(const std::string nickname);
	void				set_username(const std::string username);
	void				set_operator(const bool to_op);
	void				set_registered(const bool connected);
	void				set_pass(const std::string passord);
	void				set_realname(const std::string real_name);
	void				clear_cmd(void);

	private:

	std::string			nick;
	std::string			host;
	std::string			user;
	std::string			mode;
	std::string			unused;
	std::string			realname;
	std::string			pass;
	int					fd;
	char				buf[1000];
	std::string			save;
	std::string			cmd;
	bool				_emptyPassWord;
	bool				_operator;
	bool				_connected;

};

std::ostream &			operator<<( std::ostream & o, Client const & i );

#endif /* ********************************************************** CLIENT_H */