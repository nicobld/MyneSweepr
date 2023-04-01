PROG=server_MyneSweepr


INCLUDES=$(shell find . -name "*.h")
OBJS=$(subst .cpp,.o,$(shell find . -name "*.cpp"))
LIST=$(INCLUDES) $(OBJS)

all: $(PROG)

$(PROG): $(LIST)
	$(CC) $(OBJS) -o $@ $(CFLAGS) $(CPPFLAGS) $(LDLIBS)

%.o : %.cpp
	$(CC) -c $< -o $@ $(CFLAGS) $(CPPFLAGS) $(LDLIBS) -MMD

clean:
	rm -f $(OBJS)
	rm -f $(PROG)
	rm -f $(OBJS:.o=.d)

-include $(OBJS:.o=.d)