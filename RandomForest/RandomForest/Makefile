CFLAGS = `pkg-config --cflags opencv`
LIBS = `pkg-config --libs opencv`
OTHERLIBS = '-I /usr/include/boost'

% : %.cpp
	g++ $(CFLAGS) $(LIBS) $(OTHERLIBS) -o $@ $<
