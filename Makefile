all:
	@test -s build || { mkdir -p build && cd build && cmake ..; }
	@cd build && $(MAKE)
	@build/example/example

clean:
	rm -rf build
