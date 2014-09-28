TARGETS=main skill_data box_data decode
OBJS=bin_decode.o monster_data.o

all: $(TARGETS)

card_data.o: enum.h

$(patsubst %, %.o, $(TARGETS)): %.o: %.cpp bin_decode.h
	$(CXX) $(CXXFLAGS) -c -o $@ $<


$(TARGETS): %: %.o $(OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $^

clean:
	rm -f $(TARGETS) *.o
