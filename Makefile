NAME=mandelbrot
SRCS_IMGUI=lib/imgui/imgui.cpp lib/imgui/imgui_draw.cpp lib/imgui/imgui_tables.cpp lib/imgui/imgui_widgets.cpp lib/imgui/imgui-SFML.cpp
OBJS_IMGUI=$(addprefix obj/imgui/,$(subst .cpp,.o,$(notdir $(SRCS_IMGUI))))
SRCS=src/main.cpp src/Mandelbrot.cpp src/MandelbrotSingleThread.cpp src/MandelbrotOpenMP.cpp src/MandelbrotThread.cpp
OBJS=$(subst src,obj,$(subst .cpp,.o,$(SRCS)))
INCLUDES=include lib/imgui
FLAGS=-lsfml-window -lsfml-system -lsfml-graphics -lglfw -lGL -fopenmp -mavx -march=native -I includes
CC=g++

$(NAME): $(OBJS) $(OBJS_IMGUI)
	$(CC) $(OBJS_IMGUI) $(OBJS) -o bin/$(NAME) $(FLAGS)

$(OBJS): obj/%.o: src/%.cpp
	$(CC) -c $< -o $@ $(FLAGS)

$(OBJS_IMGUI): obj/imgui/%.o: lib/imgui/%.cpp
	$(CC) -c $< -o $@ $(FLAGS)

clean:
	rm -f bin/$(NAME)

fclean: clean
	rm -f obj/*.o
	rm -f obj/imgui/*.o

re: fclean $(NAME)

build:
	mkdir -p ./bin
	mkdir -p ./obj
