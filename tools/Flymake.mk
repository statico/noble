#
# Symlink this to ../Makefile to use with Flymake.
#

SLANG_FLAGS=-I/opt/local/include  -L/opt/local/lib -lslang -ldl -lm

.PHONY: check-syntax
check-syntax:
	$(CXX) -g -Wall -Werror -pedantic -fvisibility=hidden -m64 -fsyntax-only -Isrc -Ideps/v8/src $(SLANG_FLAGS) $(CHK_SOURCES)
