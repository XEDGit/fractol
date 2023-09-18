HEADERS := includes/fractol.h

SRC_DIR := src

OBJ_DIR := obj

FILES :=  $(notdir $(wildcard $(SRC_DIR)/*.c))

MLX := build/libmlx42.a

INC := -Iincludes -IMLX42/include

OPT := -flto -O3

ifeq ($(shell uname -s),Linux)
	LIBFLAGS := -ldl -lglfw -pthread -lm
else
	LIBFLAGS := -L "/Users/$(USER)/.brew/lib" -lglfw -framework Cocoa -framework OpenGL -framework IOKit
endif

CFLAGS := $(INC) -Wall -Wextra -Werror -c $(OPT)

LFLAGS := $(INC) $(LIBFLAGS) $(OPT)

SRC := $(addprefix $(SRC_DIR)/, $(FILES))

OBJ := $(addprefix $(OBJ_DIR)/, $(FILES:.c=.o))

GREEN := \033[32m

DEFAULT := \033[39m

NAME := fractol

all: $(NAME)

$(NAME): $(MLX) $(OBJ)
	gcc $(OBJ) $(MLX) $(DEBUG) $(LFLAGS) -o $@

$(OBJ_DIR):
	mkdir -p $@

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.c $(HEADERS) | $(OBJ_DIR)
	$(CC) $< $(CFLAGS) $(DEBUG) -o $@

$(MLX):
	cmake -S MLX42 -B build
	cmake --build build -j4

clean:
	rm -rf $(OBJ_DIR)
fclean:
	rm -rf $(OBJ_DIR) $(NAME) build

re: clean all

d: DEBUG = -g3 -fsanitize=address
d: LFLAGS = $(INC) $(LIBFLAGS)
d: CFLAGS := $(INC) -Wall -Wextra -Werror -c
d: $(1)

t: DEBUG = -g3 -fsanitize=thread
# t: LFLAGS = $(INC) $(LIBFLAGS)
# t: CFLAGS := $(INC) -Wall -Wextra -Werror -c
t: $(1)

s: DEBUG = -g3
s: LFLAGS = $(INC) $(LIBFLAGS)
s: CFLAGS := $(INC) -Wall -Wextra -Werror -c
s: $(1)

bench: DEBUG = -D BENCH
bench: $(1)

bench_auto: DEBUG = -D BENCH_AUTO
bench_auto: $(1)

mem:
	memdetect $(SRC) $(MLX) $(LFLAGS) $(1)