game:game.o main.o probar.o
	gcc $^ -o $@

%.o:%.c
	gcc -c $<


.PHONY:clean
clean:
	rm -f *.o game
