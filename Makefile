CC= c++

FLAGS= -Wall -Wextra -Werror 
# can compile following the C++98 standard (C++ first version)
EXTRA=  -std=c++98 #-g -fsanitize=address

SRC_PATH= ./src/
OBJ_PATH= ./objects/
INC= -I ./includes/

SRC= main.cpp\
	Server.cpp\
	User.cpp\
	Channel.cpp\
	helperFunctions.cpp\
	clientReplies.cpp\
	Exceptions.cpp\
	validationUtils.cpp\
	authentification.cpp\
	parsingUtils.cpp\
	debug.cpp\
	globalCommands.cpp\
	ircGod.cpp\
	commandsPart1.cpp\
	commandsPart2.cpp\
	commandsPart3.cpp\
	JoinAndPart.cpp \
	AuthComands.cpp\
	privmsgNotice.cpp\
	quit.cpp\
	bot.cpp\


NAME= ircserv

# OBJECT FILES STRING AND PREFIX CREATION
OBJ= $(SRC:.cpp=.o)
OBJS=$(addprefix $(OBJ_PATH), $(OBJ))

# OBJECT FILES CREATION (dependecies: source files and header files)
$(OBJ_PATH)%.o: $(SRC_PATH)%.cpp
	@$(CC) -c $(INC) $< -o $@  

All: $(OBJ_PATH) $(NAME) 
	@echo "\033[1m\033[33m███████╗████████╗░░░░░░░░██╗██████╗░░█████╗░"
	@echo "██╔════╝╚══██╔══╝░░░░░░░░██║██╔══██╗██╔══██╗"
	@echo "█████╗░░░░░██║░░░░░░░░░░░██║██████╔╝██║░░╚═╝"
	@echo "██╔══╝░░░░░██║░░░░░░░░░░░██║██╔══██╗██║░░██╗"
	@echo "██║░░░░░░░░██║░░░██████╗░██║██║░░██║╚█████╔╝"
	@echo "╚═╝░░░░░░░░╚═╝░░░╚═════╝░╚═╝╚═╝░░╚═╝░╚════╝░\033[0m"


# CREATE OBJECT DIRECTORY
$(OBJ_PATH):
	@mkdir -p $(OBJ_PATH)

# BUILDING THE EXECUTABLE
$(NAME): $(OBJS)
	@$(CC) $(FLAGS) $(EXTRA) $(OBJS) -o $@

clean:
	@rm -fr $(OBJ_PATH)

fclean: clean
	@rm -f $(NAME)

re: fclean All

.PHONY: clean fclean All