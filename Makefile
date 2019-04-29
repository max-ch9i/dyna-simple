LINK.o = $(CXX) $(CXXFLAGS) $(LDFLAGS) $(TARGET_ARCH)
CXXFLAGS = -std=c++11 -g -I/usr/X11/include -O0 -Wall -fPIC
LDFLAGS= -rpath /usr/X11/lib \
				 -L/usr/X11/lib \
				 -lcairo \
				 -lX11

SOURCES = main.cpp \
					Dyna.cpp \
					Cracker.cpp \
					Balloon.cpp \
					Character.cpp \
					Game.cpp \
					Utils.cpp \
					AI_Balloon.cpp \
					map_generator.cpp \
					writer.cpp \
					game_writer.cpp \
					game_reader.cpp \
					read_rep.cpp

main: main.o Dyna.o Cracker.o Balloon.o Character.o Game.o Utils.o AI_Balloon.o map_generator.o writer.o game_writer.o

read: read_rep.o writer.o game_reader.o
	$(LINK.o) $^ $(LOADLIBES) $(LDLIBS) -o $@

.PHONY: clean
clean:
	rm *.o *.d

include $(subst .cpp,.d,$(SOURCES))

%.d: %.cpp
	$(CXX) -M $(CXXFLAGS) $< > $@.$$$$; \
	sed 's,\($*\)\.o[ :]*,\1.o $@ : ,g' < $@.$$$$ > $@; \
	rm -f $@.$$$$
