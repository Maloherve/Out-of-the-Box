# Makefile for quantumsim 1.0 by Raffaele Ancarola 
# C++ library for quantum mechanics simulations

NAME := quantumsim
BUILD:= build
BIN := $(BUILD)/$(NAME).a
CC := g++
STDCPP := -std=c++17
FLAGS:= -fPIC

BACKUP := backup

INCLUDES:= -I.
DIRS:= grid

# other required libraries
LIBS := #add libraries

SRC := \
	grid/wave.cpp \
	grid/qsystem1D.cpp

OBJ := $(patsubst %.cpp,$(BUILD)/%.o,$(SRC))

.PHONY: dirs clean backup restore
all: $(BIN)

# builds all binaries into the shared library

$(BIN): dirs $(OBJ) 
	@printf "\nAssembling binaries\n\n"
	ar rvs -o $@ $(OBJ)
	@printf "\nCompilation successfully completed\n"

# compile all sources

$(OBJ): $(BUILD)/%.o : %.cpp $(SRC)
	@printf "\nCompiling $<\n"
	$(CC) -c $< ${FLAGS} -o $@ $(INCLUDES) $(STDCPP)

# generate all necessaries directories
dirs:
	mkdir -p $(BUILD) $(patsubst %,$(BUILD)/%,$(DIRS))
	@printf "Default directories created\n"

# clean all binaries
clean:
	rm -rfv $(BUILD)/*
	@printf "Binary files cleaned\n"

# backup the project in backup/symkit.zip
backup:
	mkdir -p $(BACKUP)
	rm -rfv $(BACKUP)/*
	zip -r $(BACKUP)/$(NAME).zip $(DIRS)
	@printf "Backup completed\n"

# restore the last backup, backup/symkit.zip must be present
restore:
	unzip $(BACKUP)/$(NAME).zip -d $(BACKUP)
	rm -rfv $(DIRS)
	cp $(patsubst %,$(BACKUP)/%,$(DIRS)) . 
	@printf "Backup restored\n"

