PROG=client_MyneSweepr

LIBS:=$(LIBS) -lSDL2main -lSDL2

INCLUDES=$(shell find . -name "*.h")
OBJS=$(subst .cpp,.o,$(shell find . -name "*.cpp"))
LIST=$(INCLUDES) $(OBJS)

all: $(PROG)

$(PROG): $(LIST)
	$(CC) $(OBJS) -o $@ $(CFLAGS) $(CPPFLAGS) $(LIBS)

%.o : %.cpp
	$(CC) -c $< -o $@ $(CFLAGS) $(CPPFLAGS) $(LIBS) -MD

clean:
	rm -f $(OBJS)
	rm -f $(PROG)
	rm -f $(OBJS:.o=.d)

-include $(OBJS:.o=.d)