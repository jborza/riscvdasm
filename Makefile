CXX=clang

all: compile

compile:
	$(CXX) -g -o riscvdasm -Wall *.c

web:
	cd web; \
	emcc ../*.c -o auto_em.js --pre-js=pre.js -s "EXPORTED_FUNCTIONS=['_disassemble_file']" -s "EXTRA_EXPORTED_RUNTIME_METHODS=['ccall']" -s FORCE_FILESYSTEM=1; \
	cd ..

.PHONY: web