TARGET := consbri
CFLAGS := -Werror -Wall -std=c99

all: $(TARGET)

$(TARGET): $(TARGET).o


clean:
	rm -f *.o core $(TARGET)

