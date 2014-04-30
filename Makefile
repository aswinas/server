CFLAGS=	-I.. -pthread -g

all:
	OS=`uname`; \
	  test "$$OS" = Linux ; \
	  $(CC) $(CFLAGS) ashserver.c -o server ;

