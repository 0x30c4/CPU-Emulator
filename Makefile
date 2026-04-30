
make_mem_build:
	gcc -o ./bin/make_mem ./make_memory_dump.c

make_mem: make_mem_build
	./bin/make_mem ./bin/memory.bin

build:
	gcc -o ./bin/main main.c

run_cpu: make_mem build
	./bin/main ./bin/memory.bin
