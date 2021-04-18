CXX=clang

all: compile

compile:
	$(CXX) -g -o riscvdasm -Wall *.c

web:
	cd web; \
	emcc ../*.c -O3 -o auto_em.js --pre-js=pre.js -s "EXPORTED_FUNCTIONS=['_disassemble_file']" -s "EXTRA_EXPORTED_RUNTIME_METHODS=['ccall']" -s FORCE_FILESYSTEM=1; \
	cd ..

web_single:
	cd web; \
	emcc ../*.c -O3 -o auto_em.js --pre-js=pre.js --memory-init-file 0 -s "EXPORTED_FUNCTIONS=['_disassemble_file']" -s "EXTRA_EXPORTED_RUNTIME_METHODS=['ccall']" -s FORCE_FILESYSTEM=1 -s WASM=0; \
	cd ..
	
.PHONY: web web_single