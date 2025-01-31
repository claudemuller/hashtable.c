build:
	clang -std=c99 -g -Wall -fsanitize=address -fno-omit-frame-pointer -o bin\ht.exe src\main.c -static-libasan
