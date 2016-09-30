CFLAGS = -lpthread -lstdc++
CFLAGS += `pkg-config --libs opencv`

crop: crop.cpp
	g++ $(CFLAGS) crop.cpp -o crop

straighten-img: straighten-img.cpp
	g++ $(CFLAGS) straighten-img.cpp -o straighten-img

