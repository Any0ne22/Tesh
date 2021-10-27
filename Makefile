# Generals
CC      = gcc
CFLAGS  = -std=gnu11 -Wall -pedantic -O3 -g 
LEAKDETECTION = -fsanitize=address -fsanitize=leak -fno-omit-frame-pointer
SNOWFLAGS = -Isnow -DSNOW_ENABLED


# Files
DIR = src
SOURCES = process.c
TESHFILE = tesh.c
SNOWFILE = tests.c

all: tesh

# Targets
tesh-leak-detection:
	$(CC) $(CFLAGS) $(LEAKDETECTION) -o tesh $(addprefix $(DIR)/,$(SOURCES)) $(DIR)/$(TESHFILE)

test-leak-detection:
	$(CC) $(SNOWFLAGS) $(LEAKDETECTION) $(CFLAGS) -o test $(addprefix $(DIR)/,$(SOURCES)) $(DIR)/$(SNOWFILE)

# Leak detection doesn't work in docker
tesh:
	$(CC) $(CFLAGS) -o tesh $(addprefix $(DIR)/,$(SOURCES)) $(DIR)/$(TESHFILE)

test:
	$(CC) $(SNOWFLAGS) $(CFLAGS) -o test $(addprefix $(DIR)/,$(SOURCES)) $(DIR)/$(SNOWFILE)


clean:
	rm -rf tesh test

.PHONY: all tesh test clean
