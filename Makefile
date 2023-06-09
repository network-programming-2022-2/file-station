CC = gcc
CFLAGS = -I/usr/include/postgresql -std=c99
LDFLAGS = -lpq

SRCDIR = src
OBJDIR = obj
BINDIR = bin

SERVER_SRCS = $(SRCDIR)/server.c
SERVER_OBJS = $(patsubst $(SRCDIR)/%.c, $(OBJDIR)/%.o, $(SERVER_SRCS))
SERVER_TARGET = $(BINDIR)/server

DB_SRCS = $(SRCDIR)/models/database.c
DB_OBJS = $(patsubst $(SRCDIR)/%.c, $(OBJDIR)/%.o, $(DB_SRCS))

POSTGRESQL_SRCS = $(SRCDIR)/models/postgresql/postgresql.c
POSTGRESQL_OBJS = $(patsubst $(SRCDIR)/%.c, $(OBJDIR)/%.o, $(POSTGRESQL_SRCS))

all: $(SERVER_TARGET)

$(SERVER_TARGET): $(SERVER_OBJS) $(DB_OBJS) $(POSTGRESQL_OBJS) 
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS)

$(OBJDIR)/%.o: $(SRCDIR)/%.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

$(OBJDIR)/%.o: $(SRCDIR)/models/%.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

clean:
	@rm -rf $(OBJDIR) $(BINDIR)

.PHONY: all clean

