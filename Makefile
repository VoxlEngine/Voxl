all:
	@test -s build || { mkdir -p build && cd build && cmake ..; }
	@cd build && $(MAKE) example

run:
	@test -s build || { mkdir -p build && cd build && cmake ..; }
	@cd build && $(MAKE) example
	@build/example/example

clean:
	rm -rf build
