all: OS_program2.c prod_cons_MT.c prod_cons_MT.h 
	gcc -c prod_cons_MT.c
	gcc OS_program2.c prod_cons_MT.o -o run -lpthread

#a.out: OS_program2.o prod_cons_MT.o

#gcc OS_program2.o prod_cons_MT.o

#OS_program2.o: OS_program.c prod_cons_MT.h

#gcc -c -wall OS_program2.c 

#prod_cons_MT.o: prod_cons_MT.c prod_cons_MT.h

#gcc -c -wall prod_cons_MT.c 

clean: 
	rm -f *.out *.o run


