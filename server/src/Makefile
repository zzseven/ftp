ELF:=../bin/server
OBJ:=main.c child.c pool_n.c send_fd.c
$(ELF):$(OBJ)
	gcc $^ -o $@ -I../include 

.PHONY:clean
clean:
	rm -rf ../bin/server
