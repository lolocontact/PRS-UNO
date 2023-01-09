c=gcc
op=-Wall -Wextra -lpthread

all : exe

exe : lib_sig.o lib_tube.o lib_seg.o lib_file.o uno.c lib_bal.o
	@$(c) -o exe.o -c uno.c $(op)
	@$(c) -o exe lib_sig.o lib_file.o lib_seg.o lib_bal.o lib_tube.o exe.o $(op)

lib_sig.o : libs/my_sig.c libs/libs.h libs/my_sig.h
	@$(c) -o lib_sig.o -c libs/my_sig.c $(op)

lib_tube.o : libs/my_tube.c libs/libs.h libs/my_tube.h
	@$(c) -o lib_tube.o -c libs/my_tube.c $(op)
	
lib_seg.o : libs/segment.c libs/libs.h libs/segment.h
	@$(c) -o lib_seg.o -c libs/segment.c $(op)

lib_file.o : libs/my_file.c libs/libs.h libs/my_file.h
	@$(c) -o lib_file.o -c libs/my_file.c $(op)

lib_bal.o : libs/bal.c libs/libs.h libs/bal.h
	@$(c) -o lib_bal.o -c libs/bal.c $(op)

clean :
	@rm -f exe
	rm -f lib_tube.o lib_sig.o lib_seg.o lib_file.o exe.o
