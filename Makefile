build:
	echo "> Building app"
	gcc src/main.c lib/glad/glad.c lib/maze/map.c lib/maze/stack.c -I lib -I lib/glad -I lib/KHR -I lib/maze -I lib/stb -Ilib/cglm-0.7.9/include -I include -o app -lglfw -lGL -lX11 -lpthread -lXrandr -lXi -ldl -lm -gdwarf-2
	echo "> App built"
