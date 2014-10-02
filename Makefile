OUTPUT := interpretarm.out
SRCS := $(shell find -type f -name "*.cpp")
OBJS := $(SRCS:.cpp=.o)

all:	$(OUTPUT)

$(OUTPUT):	$(OBJS)
	$(CXX) $(OBJS) -o $@

.cpp.o:
	$(CXX) -Wall -std=c++11 -c $< -o $@

clean:
	rm $(OUTPUT) $(OBJS)
