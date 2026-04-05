
.PHONY: all
all: build
	./build/room-lights -i 20239796 -p ESP03 -c

.PHONY: build
build:
	cmake -S . -B build -DCMAKE_EXPORT_COMPILE_COMMANDS=ON -DENABLE_TRACING=1
	cmake --build build

.PHONY: link_compile_commands
link_compile_commands: build
	ln -s build/compile_commands.json .

.PHONY: clean
clean:
	rm -rf ./build/
