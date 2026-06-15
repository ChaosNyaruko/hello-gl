.PHONY: show-macro

build:
	gcc  -o main   -I./glfw-3.4/include -I./include -L./glfw-3.4/build/src/  -lglfw3  -framework Cocoa -framework OpenGL -framework IOKit main.c ./src/glad.c

show-macro:
	clang -dM -E - < /dev/null
