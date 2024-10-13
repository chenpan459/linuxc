

SRC = $(wildcard *.c)
OBJ = $(patsubst %.c, %.o, $(SRC))
 
ALL:raw_socket
 
raw_socket: $(OBJ)
	gcc $< -o $@
 
$(OBJ): $(SRC)
	gcc -c $< -o $@

clean:
	rm -rf $(OBJ) raw_socket

	
.PHONY: clean ALL       

