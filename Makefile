CC= g++

FLAGS= #-Wall -Wextra -Werror 
# can compile following the C++98 standard (C++ first version)
EXTRA= -g -fsanitize=address 

SRC_PATH= ./sources/
OBJ_PATH= ./objects/
INC= -I ./includes/

SRC= main.cpp\
	Server.cpp\
	User.cpp\
	errorMsgs.cpp\
	check.cpp\


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