

C_INCS=-Iinclude
CFLAGS+=${C_INCS}
SHARED_CFLAGS=-fPIC
SHARED_LDFLAGS=-shared -Wl,-soname -Wl,

default: all

# source files
SOURCES=dlfunc.c
# dependent files
DEPENDS=$(SOURCES:.c=.d)
# object files
OBJECTS = $(SOURCES:.c=.o)

# dynamic lin library
PLATFORM_SHARED_EXT=so
SHARED_MAJOR = 1
SHARED_MINOR = 7
SHARED1 = libdlfunc.$(PLATFORM_SHARED_EXT)
SHARED2 = $(SHARED1).$(SHARED_MAJOR)
SHARED3 = $(SHARED1).$(SHARED_MAJOR).$(SHARED_MINOR)
SHARED = $(SHARED1) $(SHARED2) $(SHARED3)
$(SHARED1): $(SHARED3)
	ln -fs $(SHARED3) $(SHARED1)
$(SHARED2): $(SHARED3)
	ln -fs $(SHARED3) $(SHARED2)

# static link library
LIBRARY = libdlfunc.a


all: $(SHARED) $(LIBRARY)
	@echo $^
$(LIBRARY): $(OBJECTS)
	@rm -f $@
	ar -rs $@ $(OBJECTS)
$(SHARED3): $(SOURCES)
#	gcc -shared $(SHARED_CFLAGS) $(CFLAGS) $^ -o $@
	gcc $(SHARED_LDFLAGS)$(SHARED1) $(SHARED_CFLAGS) $(CFLAGS) $^ -o $@
#	$(CXX)  $(PLATFORM_SHARED_LDFLAGS)$(SHARED2) $(CXXFLAGS)  $(SOURCES) -o $(SHARED3) $(LIBS)

#libdlfunc.a: dlfunc.c
#	gcc ${CFLAG} -fPIC -shared $^ -o libdlfunc.so

%.o:%.c
	gcc ${CFLAGS} -c $< -o $@

clean:
	rm -f *.so *.o *.a $(SHARED) $(LIBRARY)
