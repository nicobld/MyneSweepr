PROG=client_MyneSweepr.html

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
	rm -f $(PROG:.html=.js)
	rm -f $(PROG:.html=.wasm)
	rm -f $(PROG:.html=.data)

-include $(OBJS:.o=.d)
