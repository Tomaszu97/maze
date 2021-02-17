build:
	echo "> Building app"
	gcc src/main.c src/glad.c -I include -o app -lglfw -lGL -lX11 -lpthread -lXrandr -lXi -ldl
	echo "> App built"
