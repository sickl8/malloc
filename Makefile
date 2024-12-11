ifeq ($(HOSTTYPE),)
	HOSTTYPE := $(shell uname -m)_$(shell uname -s)
endif

NAME=libft_malloc.so
SONAME=libft_malloc_$(HOSTTYPE).so
OFLAGS=-Wall -Wextra -Werror -fPIC -g
SOFLAGS=-shared
CC=gcc

SRC_FOLDER=src
BUILD_FOLDER=build
SRCS=$(wildcard $(SRC_FOLDER)/*.c)
OBJS=$(patsubst $(SRC_FOLDER)/%.c,$(BUILD_FOLDER)/%.o,$(SRCS))
HEADERS=$(wildcard $(SRC_FOLDER)/*.h)
DEPS=$(OBJS:$(BUILD_FOLDER)/%.o=$(BUILD_FOLDER)/%.d)

all: $(NAME)

$(NAME): $(SONAME)
	ln -sf $(SONAME) $(NAME)

$(SONAME): $(BUILD_FOLDER) $(OBJS)
	$(CC) $(SOFLAGS) -Wl,-soname,$(SONAME) $(OBJS) -o $(SONAME)

$(BUILD_FOLDER):
	mkdir -p $(BUILD_FOLDER)

-include $(DEPS)

$(BUILD_FOLDER)/%.o: $(SRC_FOLDER)/%.c
	$(CC) $(OFLAGS) -c -o $@ $< -MMD

$(HEADERS):

clean:
	rm -rf $(OBJS) $(DEPS)

fclean: clean
	rm -rf $(NAME)
	rm -rf $(SONAME)

re: fclean all

print:
	@echo $(SRCS)
	@echo $(OBJS)
	@echo $(DEPS)
	@echo $(HEADERS)

test:
	gcc $(SRC_FOLDER)/malloc.c

stest:
	gcc $(SRC_FOLDER)/malloc.c -g -fsanitize=address

run: test
	./a.out

srun: stest
	./a.out

run_correction:
	@if [ "$$n" != "" ]; \
		then echo ---------------- compiling tests/correction/test$$n.c"..." -------------------; \
		gcc -g tests/correction/test$$n.c src/index.c src/main/*.c src/linked_list/*.c src/binary_tree/*.c src/mmap/*.c -o test$$n && \
		echo ---------------- running tests/correction/test$$n.c"..." -------------------; \
		/usr/bin/time -v ./test$$n ; \
		else echo n is an empty string \<make run_correction n\=NUM\>; fi

.PHONY: clean fclean re all print test run stest srun run_correction