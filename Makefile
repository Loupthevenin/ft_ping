# Colors
_GREY   = \033[30m
_RED    = \033[31m
_GREEN  = \033[32m
_YELLOW = \033[33m
_BLUE   = \033[34m
_PURPLE = \033[35m
_CYAN   = \033[36m
_WHITE  = \033[37m
_END    = \033[0m

NAME = ft_ping
SRC_DIR = srcs/
OBJ_DIR = objs/

# Config
SHELL = /bin/bash
CC = gcc

INCLUDE = -I includes
CFLAGS = -Wall -Werror -Wextra
RM = rm -rf
DEBUG_FLAGS = -g3

# Docker config
DOCKER_IMAGE = ft_ping_image
DOCKER_CONTAINER = ft_ping_dev

# C program
SRCS = $(shell find $(SRC_DIR) -name '*.c')
OBJS = $(patsubst $(SRC_DIR)%.c, $(OBJ_DIR)%.o, $(SRCS))

# Recipe
all: $(NAME)

$(NAME): $(OBJS)
	@printf "$(_END)\nCompiled source files\n"
	@$(CC) $(CFLAGS) $(INCLUDE) $(OBJS) -o $@
	@printf "$(_GREEN)Finish compiling $(NAME)!\n"
	@printf "Try \"./$(NAME)\" to use$(_END)\n"

$(OBJ_DIR)%.o: $(SRC_DIR)%.c
	@mkdir -p $(@D)
	@$(CC) $(CFLAGS) $(INCLUDE) -c $< -o $@
	@printf "$(_GREEN)█$(_END)"

docker-build:
	@printf "$(_CYAN)Building Docker image...$(_END)\n"
	docker build -t $(DOCKER_IMAGE) .

docker-run:
	@printf "$(_CYAN)Launching Docker container...$(_END)\n"
	docker run --rm -it \
	--name $(DOCKER_CONTAINER) \
	-v $(shell pwd):/ft_ping \
	--cap-add=NET_RAW \
	--cap-add=NET_ADMIN \
	--privileged \
	$(DOCKER_IMAGE) bash

docker: docker-build docker-run

clean:
	@printf "$(_YELLOW)Removing object files ...$(_END)\n"
	@$(RM) $(OBJ_DIR)

fclean: clean
	@printf "$(_RED)Removing object files and program ...$(_END)\n"
	@$(RM) $(NAME)

re: fclean all

debug: CFLAGS += -fsanitize=address $(DEBUG_FLAGS)
debug: re
	@printf "$(_BLUE)Debug build done$(_END)\n"

leak: CFLAGS += $(DEBUG_FLAGS)
leak: re
	@printf "$(_BLUE)Leak check build done$(_END)\n"

.PHONY: all docker clean fclean re debug leak
