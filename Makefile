CFLAGS =-Wall -Werror -Wextra 

all: cat cppcheck 

cat: clean 
	gcc $(CFLAGS) cat.c -o cat

cppcheck:
	cppcheck --enable=all --suppress=missingIncludeSystem ./cat.c

clean: 
	rm -f ./cat
