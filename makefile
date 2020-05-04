LIB := quantumgdn/quantum_al/bin/x11/libqsim.so
BINDINGS := quantumgdn/godot-cpp/bin/libgodot-cpp.linux.release.64.a
API := quantumgdn/api.json
GODOT_CPP := quantumgdn/godot-cpp
QSIM := quantumsim/bin/quantumsim.a

INSTALL_DIR := WarmUp/bin/x11

BIND_THREADS := 4

all: $(LIB)

.PHONY: install reset

$(QSIM):
	cd quantumsim && make && cd ..

$(GODOT_CPP)/SConstruct:
	mkdir -p $(GODOT_CPP)
	cd $(GODOT_CPP)
	git submodule update --init
	cd ../..

$(API): $(GODOT_CPP)
	cd $(GODOT_CPP) && \
	godot --gdnative-generate-json-api ../api.json && \
	cd ../..

$(BINDINGS): $(API)
	cd $(GODOT_CPP) && \
	scons platform=linux generate_bindings=yes -j$(BIND_THREADS)  target=release use_custom_api_file=yes custom_api_file=../api.json bits=64 && \
	cd ../..

$(LIB): $(QSIM) $(BINDINGS)
	cd quantumgdn && \
	scons platform=x11 target=release bits=64 && \
	cd ..

install:
	cp $(LIB) $(INSTALL_DIR)

clean:
	rm $(LIB)
	rm $(QSIM)
	rm quantumsim/build/evolvers/*.os
	rm quantumsim/build/grid/*.os
	rm quantumgdn/src/*.os

reset:
	$(GODOT_CPP)/SConstruct

