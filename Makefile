CFLAGS = -lpthread -lstdc++
CFLAGS += `pkg-config --libs opencv`

crop: crop.cc
	g++ $(CFLAGS) crop.cc -o crop

merge: merge.cc
	g++ -std=c++11 $(CFLAGS) merge.cc -o merge

straighten: straighten.cc
	g++ $(CFLAGS) straighten.cc -o straighten

