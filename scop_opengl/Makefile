NAME		=	scop

CC			=	g++ -std=c++17
CFLAGS		=	#-Wall -Werror -Wextra

DRAW_FLAG	=	-lm -lglfw -lGL -ldl
GLFW_LIB	=	../external/glfw
GLAD_LIB	=	../external/glad/include

INC_DIR		= 	./inc
SRC_DIR		=	./src
OBJ_DIR		=	../obj/scop_opengl
INC_ALL		=	-I$(INC_DIR) \
				-I$(GLFW_LIB) \
				-I$(GLAD_LIB) 

SRC			=	main.cpp \
				../external/glad/src/glad.c \
				Application.cpp \
				Model.cpp \
				TGAimage.cpp \
				Window.cpp \
				FPS.cpp \
				ShaderProgram.cpp \
				Input.cpp \
				VertexArray.cpp \
				VertexBuffer.cpp \
				IndexBuffer.cpp

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

$(OBJ_DIR)/%.o:	$(SRC_DIR)/%.cpp Makefile
				@mkdir -p $(@D)
				$(CC) $(CFLAGS) -c -o $@ $< $(INC_ALL)

$(NAME)		:	$(OBJ)
				$(CC) $(CFLAGS) $(OBJ) -o $(NAME) $(DRAW_FLAG) $(INC_ALL)
				@echo "\tCompiling...\t" [ $(NAME) ] $(SUCCESS)

clean		:
				@$(RM_DIR) $(OBJ_DIR)
				@echo "\tCleaning...\t" [ $(NAME) ] $(OK)

fclean		:	clean
				@$(RM_FILE) $(NAME)
				@echo "\tDeleting...\t" [ $(NAME) ] $(OK)

re			:	fclean all

.PHONY		:	all, clean, fclean, re
