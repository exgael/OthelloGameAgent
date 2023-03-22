.PHONY: all Agent BaseAgent codeEnv

all: Agent BaseAgent codeEnv

Agent:
	mkdir -p build/$@
	cmake -S $@ -B build/$@
	$(MAKE) -C build/$@

BaseAgent:
	mkdir -p build/$@
	cmake -S $@ -B build/$@
	$(MAKE) -C build/$@

codeEnv:
	mkdir -p build/$@
	cmake -S $@ -B build/$@
	$(MAKE) -C build/$@
