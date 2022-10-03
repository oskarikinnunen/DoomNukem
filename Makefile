# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: okinnune <okinnune@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2022/10/03 13:28:58 by okinnune          #+#    #+#              #
#    Updated: 2022/10/03 15:45:24 by okinnune         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #


NAME= DoomNukem
SDL2= SDL_built/lib/libSDL2.a
INCLUDE= -ISDL_built/include/SDL2/ -Isrc -Iinclude #-Ilibft
SRCFILES= main.c editor.c
SRC= $(addprefix src/,$(SRCFILES))
OBJ= $(SRC:.c=.o)
CC= gcc
CFLAGS= $(INCLUDE)

all: $(OBJ) $(SDL2)
	$(CC) $(OBJ) -o $(NAME) `SDL_built/bin/sdl2-config --cflags --libs` $(INCLUDE)

$(SDL2):
	cd SDL2-2.0.22/build && make clean &&../configure --prefix=$(PWD)/SDL_built/ && make install