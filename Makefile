pomo: pomo.c
	$(CC)  pomo.c -o pomo -lraylib -lm -ldl -lpthread -lGL -lrt -lX11 -Wall -Wextra -pedantic -std=c99
