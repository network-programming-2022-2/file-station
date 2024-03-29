CC = gcc
CFLAGS = -I/usr/include/postgresql -std=c99
LDFLAGS = -lpq

SRCDIR = src
OBJDIR = obj
BINDIR = bin
TESTDIR = test

SERVER_SRCS = $(SRCDIR)/server.c
SERVER_OBJS = $(patsubst $(SRCDIR)/%.c, $(OBJDIR)/%.o, $(SERVER_SRCS))
SERVER_TARGET = $(BINDIR)/server

CLIENT_SRCS = $(SRCDIR)/client.c
CLIENT_OBJS = $(patsubst $(SRCDIR)/%.c, $(OBJDIR)/%.o, $(CLIENT_SRCS))
CLIENT_TARGET = $(BINDIR)/client

TEST_MODEL_SRCS = $(TESTDIR)/models/pg_test.c
TEST_MODEL_OBJS = $(patsubst $(SRCDIR)/%.c, $(OBJDIR)/%.o, $(TEST_MODEL_SRCS))
TEST_MODEL_TARGET = $(BINDIR)/test/pg_test

POSTGRESQL_TEST_SRCS = $(TESTDIR)/models/postgresql.c
POSTGRESQL_TEST_OBJS = $(patsubst $(SRCDIR)/%.c, $(OBJDIR)/%.o, $(POSTGRESQL_TEST_SRCS))

POSTGRESQL_SRCS = $(SRCDIR)/models/postgresql/postgresql.c
POSTGRESQL_OBJS = $(patsubst $(SRCDIR)/%.c, $(OBJDIR)/%.o, $(POSTGRESQL_SRCS))

USER_CONTROLLER_SRCS = $(SRCDIR)/controllers/user_controller.c
USER_CONTROLLER_OBJS = $(patsubst $(SRCDIR)/%.c, $(OBJDIR)/%.o, $(USER_CONTROLLER_SRCS))

FILE_CONTROLLER_SRCS = $(SRCDIR)/controllers/file_controller.c
FILE_CONTROLLER_OBJS = $(patsubst $(SRCDIR)/%.c, $(OBJDIR)/%.o, $(FILE_CONTROLLER_SRCS))

FILE_SRCS = $(SRCDIR)/models/postgresql/file.c
FILE_OBJS = $(patsubst $(SRCDIR)/%.c, $(OBJDIR)/%.o, $(FILE_SRCS))

USER_SRCS = $(SRCDIR)/models/postgresql/user.c
USER_OBJS = $(patsubst $(SRCDIR)/%.c, $(OBJDIR)/%.o, $(USER_SRCS))

all: $(SERVER_TARGET) $(CLIENT_TARGET)

$(SERVER_TARGET): $(SERVER_OBJS) $(POSTGRESQL_OBJS) $(FILE_OBJS) $(USER_OBJS) $(USER_CONTROLLER_OBJS) $(FILE_CONTROLLER_OBJS)
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS) -pthread

$(CLIENT_TARGET): $(CLIENT_OBJS)
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -o $@ $^ $(LDFLAGS) -pthread

$(OBJDIR)/%.o: $(SRCDIR)/%.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -g -c $< -o $@

$(OBJDIR)/%.o: $(SRCDIR)/models/%.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -g -c $< -o $@

test: $(TEST_MODEL_TARGET)

$(TEST_MODEL_TARGET): $(TEST_MODEL_OBJS) $(POSTGRESQL_TEST_OBJS) $(POSTGRESQL_OBJS) $(FILE_OBJS) $(USER_OBJS)
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -g -o $@ $^ $(LDFLAGS)

clean:
	@rm -rf $(OBJDIR) $(BINDIR)

.PHONY: all clean

