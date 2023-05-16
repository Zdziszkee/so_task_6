
# rozszerzenia plików
.SUFFIXES: .c .cpp .o .x .h

# nazwa obecnego katalogu
DIR = $(notdir $(CURDIR))

# nazwa systemu operacyjnego
SYSNAME = $(shell uname -s)

#rdzenie nazw plików
NAME1 = so_task_6
NAME2 = duplicator
NAME3 = simple_semaphore
TEXT_FILE = numer.txt
PROCESSES = 1
SECTIONS = 3
SEMAPHORE_NAME = 2
# 1->synchro on 0->synchro off
SYNCHRONIZATION = 1;


# Programy wykonawcze
EXEC1  = $(NAME1).x
EXEC2  = $(NAME2).x


# object files
OBJS1  = $(NAME1).o
OBJS2  = $(NAME2).o
OBJS3  = $(NAME3).o

# Pliki naglowkowe
HEADS1 = $(NAME3).h

# Nazwa biblioteki
LIB_NAME = $(NAME3)

# Katalog dla bibliotek
LIB_DIR = ./lib

# Biblioteka statyczna
LIB_STAT = lib$(LIB_NAME).a

# Pliki skompilowane wchodzace do biblioteki
LIB_OBJS = $(OBJS3)


# Opcje kompilatora i linkera; -fPIC potrzebna dla bibliotek dzielonych
CFLAGS = -Wall -std=c99 -pedantic -O -fPIC
LFLAGS = -Wall -std=c99 -pedantic -O

# Kompilator i linker
CO = gcc
LD = $(CO)
# Program-archiwizer do tworzenia bibliotek statycznych
AR = ar
# Opcje archiwizera: tworzenie/uaktualnianie biblioteki wraz z indeksem plikow
#                    i wypisywanie komunikatow
ARFLAGS = rsv


# reguła wzorcowa
%.o: %.c %.h
	$(CO) $(CFLAGS) -c $<
%.o: %.c
	$(CO) $(CFLAGS) -c $<


# biblioteka statyczna
$(LIB_STAT): $(LIB_OBJS)
	$(AR) $(ARFLAGS) $@ $?
	mkdir -p $(LIB_DIR)
	mv $(LIB_STAT) $(LIB_DIR)


# programy wykonawcze
$(EXEC2): $(OBJS2) $(LIB_STAT)
	$(LD) -o $@ $(LFLAGS) $(OBJS2) -L$(LIB_DIR) -l$(LIB_NAME)

$(EXEC1): $(OBJS1) $(LIB_STAT)
	$(LD) -o $@ $(LFLAGS) $(OBJS1) -L$(LIB_DIR) -l$(LIB_NAME)
	rm -rf $(LIB_DIR)


.PHONY: run

# uruchomienie programu To run program correctly type: ./programName program numberOfProcesses numberOfSections textFile semaphoreName synchronization");
run: ./$(EXEC2) ./$(EXEC1) $(PROCESSES) $(SECTIONS) $(TEXT_FILE) $(SEMAPHORE_NAME) $(SYNCHRONIZATION)

.PHONY: clean tar

# czyszczenie katalogu
clean:
	rm -f *.o  *~ *.a *.so *.x core core* a.out; rm -rf ${LIB_DIR}

# Archiwizacja i kompresja
tar: clean
	(cd ../; tar -cvzf $(DIR).tar.gz  $(DIR) )