TARGETS=card_data skill_data decode
OBJS=bin_decode.o

all: $(TARGETS)

$(TARGETS): %: %.cpp $(OBJS)


