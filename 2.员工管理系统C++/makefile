target=main
cc=g++  
obj=1main.o bose.o manager.o emloyee.o workerManager.o
cflags=-Wall -g

$(target):$(obj)
	$(cc) $^ -o $@ $(cflags)
%.o:%.c
	$(cc) -c $< -o &@ $(cflags)

clean:
	rm $(target) *.o a.exe a.out
cleano:
	rm *.o
