#How to compile so: g++ -shared -fPIC -o libname.so add.cc sub.cc mul.cc div.cc
LIB_OBJS = time.cc
LIB_CFLAGS = -shared -fPIC
LIB_NAME = libtime.so
OBJS = Log.cc Configure.cc MyJob.cc Job.cc WorkThread.cc ThreadPool.cc ThreadManage.cc main.cc
CC = g++
CFLAGS = -d
LIBPATH = .
LIBRARY = $(LIBPATH)/libtime.so
default:$(OBJS)
	$(CC) -g $(LIB_OBJS) -o $(LIB_NAME)
	$(CC) -g $(OBJS) $(LIBRARY) -o run -lpthread
clean:
	rm run $(LIB_NAME)
