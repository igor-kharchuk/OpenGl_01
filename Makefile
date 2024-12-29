CC = gcc
CFLAGS = -Wall -MMD -O2
LDFLAGS = -lglfw -lGL -lGLEW -lm -lcglm -lfreeimage

# Директори для об'єктних файлів і результатів
OBJDIR = build/obj
DEPDIR = build/deps
BINDIR = build/bin

# Знаходження всіх файлів з розширенням .c в директорії src
SRC = $(wildcard src/*.c)
OBJ = $(SRC:src/%.c=$(OBJDIR)/%.o)
DEPS = $(SRC:src/%.c=$(DEPDIR)/%.d)
TARGET = $(BINDIR)/program

.PHONY: all clean test

# Головна ціль - виконуваний файл
all: $(TARGET)

# Правило створення виконуваного файлу
$(TARGET): $(OBJ)
	@mkdir -p $(BINDIR)  # Створення директорії для виконуваного файлу, якщо її немає
	$(CC) $(CFLAGS) $(OBJ) $(LDFLAGS) -o $(TARGET)

# Правило створення об'єктних файлів
$(OBJDIR)/%.o: src/%.c
	@mkdir -p $(OBJDIR)  # Створення директорії для об'єктних файлів, якщо її немає
	$(CC) $(CFLAGS) -c $< -o $@

# Включення залежностей
-include $(DEPS)

# Очищення всього
clean:
	rm -rf $(OBJDIR) $(DEPDIR) $(BINDIR)

test:
	./build/bin/program
