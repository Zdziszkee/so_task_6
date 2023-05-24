#
# make: all, clean, tar
#
#=========================================================================
DIR = `basename $(CURDIR)`
#------------------------

NAME1 = duplicator
NAME2 = so_task_6
LIB1 = simple_semaphore
FILE = number.txt

NUMBER_OF_SECTIONS = 7
NUMBER_OF_PROGRAMS = 5
SEMAPHORE_NAME = sem
SYNCHRONIZATION = 1

#----------------------
EXEC1 = $(NAME1)
OBJS1 = $(NAME1).o
#----------------------
EXEC2 = $(NAME2)
OBJS2 = $(NAME2).o
#----------------------
LIBS1 = $(LIB1).c
#----------------------
#############################
COFLAGS = -lpthread -Wall -std=c99 -pedantic
LDFLAGS = -lpthread -Wall -std=c99 -pedantic
CO = gcc
LD = $(CO)
#############################
%.o: %.c
	$(CO) $(COFLAGS) -c $<
#############################
.PHONY: all
all: $(EXEC1) $(EXEC2)

.PHONE: d
d: $(EXEC1)

.PHONE: i
i: $(EXEC2)
#############################
$(EXEC1): $(OBJS1) $(LIBS1)
	 $(LD) $^ -o $@ $(LDFLAGS)
#############################
$(EXEC2): $(OBJS2) $(LIBS1)
	 $(LD) $^ -o $@ $(LDFLAGS)
#############################

.PHONE: run
run: $(EXEC1) $(EXEC2)
	clear
	./$(EXEC1) ./$(EXEC2).x $(NUMBER_OF_PROGRAMS) $(NUMBER_OF_SECTIONS) $(FILE) $(SEMAPHORE_NAME) $(SYNCHRONIZATION)

.PHONY: clean tar
EXECS = $(EXEC1) $(EXEC2)

clean:
	rm -f *.o  *~ $(EXECS)
#=========================================================================
# Archiwizacja i kompresja
tar: clean
	(cd ../; tar -cvzf $(DIR).tar.gz  $(DIR) )
#=========================================================================
