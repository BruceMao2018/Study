#How to compile so: g++ -shared -fPIC -o libname.so add.cc sub.cc mul.cc div.cc
OBJS = Log.cc Configure.cc MyJob.cc Job.cc WorkThread.cc ThreadPool.cc ThreadManage.cc main.cc
CC = g++
CFLAGS = -d
LIBRARY = ./libtime.so
default:$(OBJS)
	$(CC) $(CLFAGS) $(OBJS) $(LIBRARY) -o run -lpthread
clean:
	rm run
