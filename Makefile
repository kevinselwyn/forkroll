CC    := gcc
NAME  := forkroll
FLAGS := -Wall -Wextra

all: $(NAME)

$(NAME): $(NAME).c
	$(CC) -o $@ $< $(FLAGS)

test: $(NAME)
	./$(NAME)

clean:
	rm -f $(NAME)