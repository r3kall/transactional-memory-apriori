
DIR = src

.PHONY: source-dir

source-dir:
	$(MAKE) -C $(DIR)

clean:
	rm -rf *~ *.o offsets && $(MAKE) -C $(DIR) clean

# offsets.c is a program to generate the offset file
offsets: offsets.c
	$(CXX) -g $< -o $@