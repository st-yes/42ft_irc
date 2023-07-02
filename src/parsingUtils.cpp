#include "../includes/Ft_irc.hpp"

int	countParams(std::string	command)
{
	int params;

	params = 1;
	if (command.empty() || isAllSpace(command))
		return (0);
	for (int i = 0; i < command.length() - 1; i++)
	{
		if (command[i] == ' ' && !isspace(command[i + 1]))
		{
			params++;
			if (command[i + 1] == ':')
				break;	
		}
	}
	return (params);
}

void tokenizeCmd(std::string& cmd, std::string *params)
{
    std::istringstream iss(cmd);
    std::string 		token;
	int					i;
	bool				spaceParam;
	bool				first;

	
	i = 0;
	spaceParam = false;
    while (std::getline(iss, token, ' '))
	{
		if (!token.empty())
        {
			if (token[0] == ':')
				spaceParam = true;
			if (spaceParam == false)
			{
				params[i] = token;
				i++;
			}
			else
			{
				params[i] += token;
				params[i] += " ";
				//i++;
			}
		}
    }
}

std::string	*fillParams(std::string cmd, int size)
{
	std::string	*params;
	
	if (cmd.empty())
		return (nullptr);
	params = new std::string[size + 1];
	params[size] = "";
	tokenizeCmd(cmd, params);
	return (params);
}

void	trimLast(std::string&	str)
{
	int pos;

	pos = str.find("\r\n");
	if (pos != std::string::npos)
		str.erase(pos, 2);
}

bool	isAllSpace(std::string	str)
{
	for (int i = 0; i < str.length(); i++)
	{
		if (!std::isspace(str[i]))
			return (false);
	}
	return (true);
}

bool successiveSpace(std::string	cmd)
{
	int i;

	i = 0;
	while (i < (cmd.length() - 1) && cmd[i] != ':')
	{
		if (cmd[i] == ' ' && cmd[i + 1] == ' ')
			return (true);
		i++;
	}
	return (false);
}

std::string*	allocateForParams(int size)
{
	std::string	*param = new std::string[size + 1];
	param[size] = "";
	return (param);  
}