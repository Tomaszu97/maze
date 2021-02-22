build:
	echo "> Building app"
	gcc -g src/main.c lib/glad/glad.c -I lib -I lib/glad -I lib/KHR -I lib/stb -Ilib/cglm-0.7.9/include -I include -o app -lglfw -lGL -lX11 -lpthread -lXrandr -lXi -ldl -lm
	echo "> App built"
