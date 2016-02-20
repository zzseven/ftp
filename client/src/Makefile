ELF:=../bin/client
OBJ:=client.c  operate_file.c
$(ELF):$(OBJ)
	gcc $^ -o $@ -I../include 

.PHONY:clean
clean:
	rm -rf ../bin/client
