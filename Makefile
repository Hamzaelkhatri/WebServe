NAME= Webserv
COMP= clang++
# FLAGS= -Wall -Wextra -Werror

SRC_PATH= ./src
HDR_PATH= ./includes
OBJ_PATH= obj

SRC_NAME= main.cpp\
		  ErrorHandling.cpp\
		  tools.cpp\
		  server.cpp\
		  pars.cpp\

OBJ_NAME= $(SRC_NAME:.cpp=.o)

OBJ= $(addprefix $(OBJ_PATH)/,$(OBJ_NAME))
SRC= $(addprefix $(SRC_PATH)/,$(SRC_NAME))
HDR= $(addprefix $(HDR_PATH)/,$(HDR_NAME))

H_FLAG= -I $(HDR_PATH)


all: $(NAME) 

$(NAME) : $(OBJ)
	@rm -rf $(NAME)
	@$(COMP) -g $(H_FLAG) $(OBJ) -o $@
	@echo "\033[91m \t this project Made by: Helkhatri && Zdnaya \n\033[0m"                                          
	@echo "\033[91m \t Webserve has been created successfully  \n\033[0m"


$(OBJ_PATH)/%.o:  $(SRC_PATH)/%.cpp $(HDR)
	@mkdir -p $(OBJ_PATH) 
	@$(COMP) -g $(FLAGS) $(H_FLAG) -g -o $@ -c $<

clean:
	@rm -rf $(OBJ_PATH)
	@echo "\033[10m  all objects files are deleted.\n\033[0m" 

fclean: clean
	@rm -rf $(NAME)
	@echo "\033[10m  $(NAME) && all obbjects are deleted.\n\033[0m" 

re : fclean all