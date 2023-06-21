NAME		=	scop

SOFT_RENDER	=	scop_soft_render

RM_DIR		=	rm -rf
RM_FILE		=	rm -f

#COLORS
C_NO		=	"\033[00m"
C_OK		=	"\033[32m"
C_GOOD		=	"\033[32m"

#DEBUG
SUCCESS		=	$(C_GOOD)SUCCESS$(C_NO)
OK			=	$(C_OK)OK$(C_NO)

all			:
				@make -C ${SOFT_RENDER}/ all
				@cp ${SOFT_RENDER}/${NAME} ${NAME}

clean		:
				@$(RM_DIR) $(OBJ_DIR)
				@echo "\tCleaning...\t" [ $(NAME) ] $(OK)

fclean		:	clean
				@$(RM_FILE) $(NAME)
				@echo "\tDeleting...\t" [ $(NAME) ] $(OK)

re			:	fclean all

.PHONY		:	all, clean, fclean, re
