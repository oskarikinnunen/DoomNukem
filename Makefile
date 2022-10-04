# **************************************************************************** #
#                                                                              #
#                                                         :::      ::::::::    #
#    Makefile                                           :+:      :+:    :+:    #
#                                                     +:+ +:+         +:+      #
#    By: okinnune <okinnune@student.42.fr>          +#+  +:+       +#+         #
#                                                 +#+#+#+#+#+   +#+            #
#    Created: 2022/10/03 13:28:58 by okinnune          #+#    #+#              #
#    Updated: 2022/10/04 11:28:44 by okinnune         ###   ########.fr        #
#                                                                              #
# **************************************************************************** #


NAME= DoomNukem
#Library dependencies:
SDL2= SDL_built/lib/libSDL2.a
LIBFT= libft/libft.a

#Source files:
SRCFILES= main.c draw.c v2.c\
		editor.c editor_mouse.c editor_events.c editor_render.c editor_saveline.c editor_map_io.c #EDITOR FILES
SRC= $(addprefix src/,$(SRCFILES))
OBJ= $(SRC:.c=.o)

#Compilation stuff:
INCLUDE= -ISDL_built/include/SDL2/ -Isrc -Iinclude -Ilibft $(LIBFT) 
CC= gcc
CFLAGS= $(INCLUDE) -lm -g

all: $(OBJ) $(SDL2) $(LIBFT)
	$(CC) $(OBJ) -o $(NAME) `SDL_built/bin/sdl2-config --cflags --libs` $(INCLUDE)

$(OBJ): include/doomnukem.h

clean:
	rm -f src/*.o

re: clean all

$(LIBFT):
	make -C libft

$(SDL2):
	cd SDL2-2.0.22/build && make clean &&../configure --prefix=$(PWD)/SDL_built/ && make install