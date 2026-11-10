#------------------------------------------------------------------------------
#
# Makefile --- Demonstrate how to rebuild *.o files if a *.h file changes
#
# Note:  This approach overrides make's built-in rule for compiling *.c files
#
# Usage
#	make all	Build the released product
#	make clean	Remove build artifacts
#
# Author(s)
# 01/24/18 Epoch............................................................jrc
#
#------------------------------------------------------------------------------

#-------Define names of all the object files in this project
	OBJS = smash.o  commands.o history.o

	 HDRS = smash.h history.h

#-------Define the name of the resulting released product
	EXE = smash

#-------Define options passed by make to the compiler
	CFLAGS = -Wall -std=c99

#-------Define the name of the compiler to be used
	CC = gcc

#-------Override the rule for compiling *.c files to consider *.h dependencies
%.o: %.c $(HDRS)
	$(CC) -c $(CFLAGS) $< -o $@
	


#Arrange for phony target all to rebuild the dependency file, rules.d
all:  rules.d smash

debug: CFLAGS += -g -o0
debug: smash

# rebuild
rules.d: $(wildcard *.c) $(wildcard *.h)
	gcc -MM $(wildcard *.c) >rules.d

	#Incorporate the auto-generated dependencies from rules.d into this Makefile
-include rules.d

#-------Define target "all" for building the executable(s)
all:	$(EXE)



#-------Rule for linking the executable product
$(EXE): $(OBJS) smashLib.a
	$(CC) $(CFLAGS) $^ -g -o $@

smashLib.a: $(OBJS) 
	ar rcs smashLib.a $(OBJS)
libs: smashLib.a
valgrind: debug
	valgrind --leak-check=yes --leak-check=full --show-leak-kinds=all  smash

#-------Rule for cleaning build and emacs artifacts
clean:
	rm -f $(OBJS) $(EXE) *.d *.a