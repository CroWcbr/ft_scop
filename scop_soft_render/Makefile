NAME		=	scop

CC			=	g++ -std=c++17
CFLAGS		=	#-Wall -Werror -Wextra

DRAW_FLAG	=	-lm -lglfw -lGL -ldl
GLFW_LIB	=	../external/glfw

INCLUDE		= 	./inc
SRC_DIR		=	./src
OBJ_DIR		=	../obj
INC_ALL		=	-I$(INCLUDE) \
				-I$(GLFW_LIB)

SRC			=	main.cpp \
				Model.cpp \
				Application.cpp \
				Window.cpp

OBJ			=	$(addprefix $(OBJ_DIR)/, $(SRC:.cpp=.o))

RM_DIR		=	rm -rf
RM_FILE		=	rm -f

#COLORS
C_NO		=	"\033[00m"
C_OK		=	"\033[32m"
C_GOOD		=	"\033[32m"

#DEBUG
SUCCESS		=	$(C_GOOD)SUCCESS$(C_NO)
OK			=	$(C_OK)OK$(C_NO)

all			:	$(NAME)

bonus		:	$(NAME)

OBJ			=	$(SRC:%.cpp=$(OBJ_DIR)/%.o)

$(OBJ_DIR)/%.o:	$(SRC_DIR)/%.cpp
				@mkdir -p $(@D)
				$(CC) $(CFLAGS) -c -o $@ $< $(INC_ALL)

$(NAME)		:	$(OBJ)
				$(CC) $(CFLAGS) $(OBJ) -o $(NAME) $(DRAW_FLAG) $(INC_ALL)
				@echo "\tCompiling...\t" [ $(NAME) ] $(SUCCESS)

clean		:
				@$(RM_DIR) $(OBJ_DIR)
				@echo "\tCleaning...\t" [ $(OBJ_DIR) ] $(OK)

fclean		:	clean
				@$(RM_FILE) $(NAME)
				@echo "\tDeleting...\t" [ $(NAME) ] $(OK)

re			:	fclean all

.PHONY		:	all, clean, fclean, re
