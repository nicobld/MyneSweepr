PROG=client_MyneSweepr

LDLIBS:=$(LDLIBS) -lSDL2main -lSDL2
INCLUDES=$(shell find . -name "*.h")
OBJS=$(subst .cpp,.o,$(shell find . -name "*.cpp"))
LIST=$(INCLUDES) $(OBJS)

all: $(PROG)

$(PROG): $(LIST)
	$(CC) $(OBJS) -o $@ $(CFLAGS) $(CPPFLAGS) $(LDLIBS) $(LDFLAGS)

%.o : %.cpp
	$(CC) -c $< -o $@ $(CFLAGS) $(CPPFLAGS) $(LDLIBS) -MMD

clean:
	rm -f $(OBJS)
	rm -f $(PROG)
	rm -f $(OBJS:.o=.d)

-include $(OBJS:.o=.d)