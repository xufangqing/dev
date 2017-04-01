
C_INCS=-Idl/include
CFLAGS+=${C_INCS}

LIB_DEPS=-ldlfunc
LIB_INCS=-Ldl

default: dl sl
	@echo dst [$^] created!

dl: main_dl_load.o dl/libdlfunc.so
	gcc $^ -ldl -o demo_dl   #dl

sl: main_static_link.o dl/libdlfunc.so
	ln -fs dl/libdlfunc.so libdlfunc.so
	gcc $^ -ldl ${LIB_INCS} ${LIB_DEPS} -o demo_sl

dl/libdlfunc.so: dl/dlfunc.c
	make -C dl
%.o : %.c
	gcc ${CFLAGS} -c $^ -o $@


clean:
	rm -f *.o *.so *.a
	rm -f demo_dl
	rm -f demo_sl
	make -C dl clean
