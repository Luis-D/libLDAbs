objs := src/ThreadPool/PThreadpool.o src/GeneralLinkedList.o src/Weak_Stack_Queue.o src/ObjectPool.o src/SinglyLinkedList.o src/DoublyLinkedList.o

projectname=LDAbs
dllname=so
staticname=a
ThreadLib=pthread
PREFIX=/usr/local
LIB_FOLDER=lib
libname=lib$(projectname)

.PHONY: all
all: archive

clean:
	- rm $(objs)

distclean: clean
	- rm -r BUILD


.PHONY: install
install:
	install -m 644 -t $(PREFIX)/include/$(projectname) -D BUILD/include/$(projectname)/*.h
	install -m 644 -t $(PREFIX)/$(LIB_FOLDER) BUILD/$(CC)/*.$(staticname)
	install -m 755 -t $(PREFIX)/$(LIB_FOLDER) BUILD/$(CC)/*.$(dllname)


uninstall:
	rm -r $(PREFIX)/include/$(projectname)
	rm $(PREFIX)/$(LIB_FOLDER)/$(libname).*

test:
	echo $(PREFIX)




BUILD:
	- mkdir BUILD
	- mkdir BUILD/include/
	- mkdir BUILD/include/$(projectname)
	- mkdir BUILD/$(CC)


archive: $(objs) BUILD
	
	$(AR) rcs BUILD/$(CC)/$(libname).$(staticname) $(objs)
	$(CC) -O3 -shared -o BUILD/$(CC)/$(libname).$(dllname) $(objs) -$(ThreadLib)
	cp src/*.h BUILD/include/$(projectname)/.


$(objs): %.o : %.c
	$(CC) -c -fPIC $< -o $@

%.c:
	touch $@

