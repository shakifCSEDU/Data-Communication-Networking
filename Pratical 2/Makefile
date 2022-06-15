ifdef OS
   RUN = .\run.exe
else
   ifeq ($(shell uname), Linux)
      RUN = ./run.out
   endif
endif

all:
	gcc main.c -o run.out
	$(RUN)
