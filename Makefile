all:
	gcc -o colision colision.c particles.c -lraylib -lGL -lm -lpthread -ldl -lrt -lX11
