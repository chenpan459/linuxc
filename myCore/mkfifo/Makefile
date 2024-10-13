

SRC = $(wildcard *.c)
OBJ = $(patsubst %.c, %.o, $(SRC))
 
ALL:mfifo
 
mfifo: $(OBJ)
	gcc $< -o $@
 
$(OBJ): $(SRC)
	gcc -c $< -o $@

clean:
	rm -rf $(OBJ) mfifo

	
.PHONY: clean ALL       

