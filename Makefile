EXEC = "melody-allocator"

$(EXEC):
	gcc -o ./$(EXEC) -O2 ./main.c

install: $(EXEC)
	cp $(EXEC) /usr/local/bin/
	chmod +x /usr/local/bin/$(EXEC)

clean:
	rm ./$(EXEC)
