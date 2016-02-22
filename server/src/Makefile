ELF:=../bin/server
OBJ:=factory.c main.c work_que.c
$(ELF):$(OBJ)
	gcc $^ -o $@ -I../include -lpthread


.PHONY:clean
clean:
	rm -rf ../bin/server
