CC:= clang -Wall -Wextra -g
mecabServer: LDLIBS += -lpthread
mecabServer: mecabServer.o
mecabServer.o:mecabServer.h