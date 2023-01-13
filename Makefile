CC=gcc
CFLAGS=-march=native

COMMANDS=$(shell ls src/)

$(COMMANDS):
	@mkdir -p out
	@$(CC) $(CFLAGS) src/$@/*.c -o out/$@
	@echo "==> command $@ is builded"

clean:
	@echo "deleting..."
	@rm -rf out
	@echo "done!"
build: $(COMMANDS)
	@echo "done!"
install:
	@echo "installing..."
	@sudo cp out/* /usr/bin/
	@echo "done!"