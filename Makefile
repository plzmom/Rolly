CC := gcc
CXX := g++

CFLAGS := -Wall -Wextra -std=c11
CXXFLAGS := -Wall -Wextra -std=c++17
INCLUDES := -Isrc -Isrc/glad/include -Isrc/lib/UI/imgui

LDLIBS := -lglfw -lGL -ldl -lm

SRCS_C := \
	src/main.c \
	src/glad/src/glad.c \
	src/lib/render/render.c \
	src/lib/win/window.c

SRCS_CPP := \
	src/lib/UI/ui.cpp \
	src/lib/UI/imgui/imgui.cpp \
	src/lib/UI/imgui/imgui_draw.cpp \
	src/lib/UI/imgui/imgui_tables.cpp \
	src/lib/UI/imgui/imgui_widgets.cpp \
	src/lib/UI/imgui/backends/imgui_impl_glfw.cpp \
	src/lib/UI/imgui/backends/imgui_impl_opengl3.cpp

OBJS := $(SRCS_C:.c=.o) $(SRCS_CPP:.cpp=.o)

src/lib/UI/ui.o: src/lib/UI/ui.cpp

TARGET := app

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) -o $@ $^ $(LDLIBS)

%.o: %.c
	$(CC) $(CFLAGS) $(INCLUDES) -c $< -o $@

%.o: %.cpp
	$(CXX) $(CXXFLAGS) $(INCLUDES) -c $< -o $@

clean:
	rm -f $(OBJS) $(TARGET)

.PHONY: all clean