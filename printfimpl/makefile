#Usual CFLAGS that you set in all of the projects.
#You may need to adjust these for different platforms
#Notice the OPTFLAGS var in the end that lets people augment
#the build options as needed.
CFLAGS=-g -O2 -Wall -Wextra -Isrc -rdynamic -DNDEBUG $(OPTFLAGS)
#These options are used when linking a library
#Someone else can then augment the linking options using the OPTLIBS
LIBS=-ldl $(OPTLIBS)
#Sets an optional var called PREFIX will only have this value if
#the person running the Makefile didn't already give a PREFIX
#setting. That's what the ?= does
PREFIX?=/usr/local
#ex:
#this installs the lib into /tmp
#make PREFIX=/tmp install
#this tells it to add pthreads
#make OPTFLAGS=-pthread

#Creates the SOURCES var by doing a wildcard search for all *.c
#files in the src/ directory. You have to give both src/**/*.c and
#src/*.c so that GNU make will include the files in src and the
#files below it.
SOURCES=$(wildcard src/**/*.c src/*.c)
#Once you ahve the list of source files, you can then
#use the patsubst to take the SOURCES list of *.c files and
#make a new list of all othe object files. You do this
#by telling patsubst to change all %.c extensions to %.o,
#and then those extensions are assigned to OBJECTS.
OBJECTS=$(patsubst %.c,%.o,$(SOURCES))

#Using wildcard again to find all of the test source
#files for the unit tests. These are separate from
#the library's source files.
TEST_SRC=$(wildcard tests/*_tests.c)
#Using patsubst trick to dynamically get all the TEST
#targets. In this case, the .c extension is stripped 
#away so that a full program will be made with the same
#name. Previously, the .c was replaced with {.o} so
#an object file is created.
TESTS=$(patsubst %.c,%,$(TEST_SRC))

#The ultimate target is build/libYOUR_LIBRARY.a,
#which you will change to be whatever library
#you are trying to build
TARGET=build/simple_test_lib.a
SO_TARGET=$(patsubst %.a,%.so,$(TARGET))

#The Target Build
#The first target is what make runs by default when no target is given.
#In this it's called all: and it gives $(TARGET) tests as the targets
#to build. Look up at the TARGET var and you see that's the library, so all
#will build the library. Thetests target is further down in the Makefile and
#builds the unit test.
all: $(TARGET) $(SO_TARGET) tests
#Here is another target for making 'developer builds' that introduces a 
#technique for changing options for just one target. If I do a 'dev build'
#I want the CFLAGS to include options like -Wextra that are useful
#for finding bugs. If you place them on the target line as options like this,
#then give another line that says the original target(in this case all),
#then it will change the options you set. I use this for setting different 
#flags on different platforms that need it.
dev: CFLAGS=-g -Wall -Isrc -Wall -Wextra $(OPTFLAGS)
dev: all

#This builds the TARGET library, whatever that is. It also uses the same
#trick from before, giving a target with just options and ways to alter
#them for this run. In this case, I'm adding -fPIC just for the library
#build, using the += syntax to add it on.
$(TARGET):CFLAGS+=-fPIC
#this is the real target, where we make the build directory, then
#compile all of the OBJECTS
$(TARGET):build $(OBJECTS)
#runs the ar command that actually makes the TARGET. The sytax $@ $(OBJECTS)
#is a way of saying, "put the taret for this Makefile source here and all
#the OBJECTS after that." IN this case, the $@ maps back to the $(TARGET) 
#from previously, which maps to build/libYOUR_LIBRARY.a. It seems like a lot
#to keep track of in this indirection, and it can be, but once you get it working,
#you just change TARGET at the top and build a whole new library.
	ar rcs $@ $(OBJECTS)
#finally, to make the lib, you run ranlib on the TARGET and it's built.
	ranlib $@
$(SO_TARGET): $(TARGET) $(OBJECTS)
	$(CC) -shared -o $@ $(OBJECTS)

build:
#makes the build/ or bin/ directories if they don't exist. This is then referenced
#from line 19 when it gives the build target to make sure the build/ dir is made.
	@mkdir -p build
	@mkdir -p bin

#The unit tests
#if there is a target that's not real, but there is a directory or file with that
#name, then you need to tag the target with .PHONY: so make will ignore the file
#and always run.
.PHONY:tests
#the same trick applies for modifying the CFLAGS var to add the TARGET to the build
# so that each of the test programs will be linked with the TARGET lib. In this case,
# it will add build/libYOUR_LIBRARY.a to the linking
tests:CFLAGS += $(TARGET)
#now we have the actual tests: target, which depends on all of the programs listed in
#the TESTS variable that we created in the header. This one line actually says, "Make,
#use what you know about build programs and the current CFLAGS settings to build each
#program in TESTS."
tests:$(TESTS)
#Finally, when all of the TESTS are built, there's a simple shell script I will create
#later that knows how to run them all and report their output. This line actually runs it
# so you can see the test results.
	sh ./tests/runtests.sh

#The Cleaner
clean:
#this cleans out most of the junk that various compliers and tools leave behind.
#It also gets rid of the build/ directory and uses a trick at the end to cleanly
#erase the weird *.dSYM directories that Apples's XCode leaves behind for debugging`
	rm -rf build $(OBJECTS) $(TESTS)
	rm -f tests/tests.log
	find . -name "*.gc*" -exec rm {} \;
	rm -rf `find . -name "*.dSYM" -print`

#The installer
#for the makefile that's build a lib, I just need to put something in the common
#PREFIX directory, usually /usr/local/lib
install:all
#this makes install: depend on the all: target, so that when you run make install,
#it will be sure to build everything.
	install -d $(DESTDIR)/$(PREFIX)/lib/
#use the program install to create the target lib dir if it doesn't exist. In this case
# try to make the isntall as flexible as possible by using two vars that are conventions
#for installers. DESTDIR is handed to make by installers, which do their builds in secure
#or odd locations, to build packages. PREFIX is used when people want the project
#to be installed in someplace other than /usr/local/.
	install $(TARGET) $(DESTDIR)/$(PREFIX)/lib/

#the checker
check:
	@echo Files with potentially dangerous functions
