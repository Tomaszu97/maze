build:
	echo "> Building app"
	gcc -g src/main.c src/glad.c -I include -o app -lglfw -lGL -lX11 -lpthread -lXrandr -lXi -ldl -lm
	echo "> App built"
