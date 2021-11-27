# Generals
CC      = gcc
CFLAGS  = -std=gnu11 -Wall -pedantic -O3 -g
LINKERFLAGS = -lreadline
LEAKDETECTION = -fsanitize=address -fsanitize=leak -fno-omit-frame-pointer
SNOWFLAGS = -Isnow -DSNOW_ENABLED


# Files
DIR = src
SOURCES = process.c tokens.c command_runner.c command_scheduler.c signals.c prompt.c builtins.c
TESHFILE = tesh.c
SNOWFILE = tests.c

all: tesh

# Targets
tesh-leak-detection:
	$(CC) $(CFLAGS) $(LEAKDETECTION) -o tesh $(addprefix $(DIR)/,$(SOURCES)) $(DIR)/$(TESHFILE) $(LINKERFLAGS)

test-leak-detection:
	$(CC) $(SNOWFLAGS) $(LEAKDETECTION) $(CFLAGS) -o test $(addprefix $(DIR)/,$(SOURCES)) $(DIR)/$(SNOWFILE) $(LINKERFLAGS)

# Leak detection doesn't work in docker
tesh:
	$(CC) $(CFLAGS) -o tesh $(addprefix $(DIR)/,$(SOURCES)) $(DIR)/$(TESHFILE) $(LINKERFLAGS)

test:
	$(CC) $(SNOWFLAGS) $(CFLAGS) -o test $(addprefix $(DIR)/,$(SOURCES)) $(DIR)/$(SNOWFILE) $(LINKERFLAGS)


clean:
	rm -rf tesh test

.PHONY: all tesh test clean
