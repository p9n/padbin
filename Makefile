
TARGETS=card_data skill_data
OBJS=bin_decode.o

all: $(TARGETS)

$(TARGETS): %: %.cpp $(OBJS)

# card_data: card_data.cpp bin_decode.o

# skill_data: skill_data.cpp bin_decode.o


