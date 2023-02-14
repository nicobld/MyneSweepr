PROG=server_MyneSweepr

INCLUDES=$(shell find . -name "*.h")
OBJS=$(subst .cpp,.o,$(shell find . -name "*.cpp"))
LIST=$(INCLUDES) $(OBJS)

all: $(PROG)

$(PROG): $(LIST)
	$(CC) $(OBJS) -o $@ $(CFLAGS) $(CPPFLAGS) $(LIBS)

%.o : %.cpp
	$(CC) -c $< -o $@ $(CFLAGS) $(CPPFLAGS) $(LIBS)

clean:
	rm -f $(OBJS)
	rm -f $(PROG)
	rm -f $(OBJS:.o=.d)