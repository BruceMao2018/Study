#How to compile so: g++ -shared -fPIC -o libname.so add.cc sub.cc mul.cc div.cc
#LIB_OBJS = time.cc
#LIB_CFLAGS = -shared -fPIC
LIB_NAME = ./libtime.so
OBJS = Job.cc MyJob.cc WorkThread.cc Log.cc pool.cc main.cc
CC = g++
CFLAGS = -d -g
default:$(OBJS)
	$(CC) -g $(OBJS) -o ttt -lpthread $(LIB_NAME)
clean:
	rm ttt $(LIB_NAME)
