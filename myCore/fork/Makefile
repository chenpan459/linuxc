

SRC = $(wildcard *.c)
OBJ = $(patsubst %.c, %.o, $(SRC))
 
ALL:fork
 
mfifo: $(OBJ)
	gcc $< -o $@
 
$(OBJ): $(SRC)
	gcc -c $< -o $@

clean:
	rm -rf $(OBJ) fork

	
.PHONY: clean ALL       

