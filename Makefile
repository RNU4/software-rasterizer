build:
	gcc -Wfatal-errors \
	-g \
	-std=c99 \
	./main.c \
	-o3 \
	-lmingw32 \
	-lSDL2main \
	-lSDL2 \
	-lSDL2_image \
	-o example.exe