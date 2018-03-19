CFLAGS = -lpthread -lstdc++
CFLAGS += `pkg-config --libs opencv`
CFLAGS += -I/Users/pks/.local/include

all: crop denoise merge straighten unsharp-mask

crop: crop.cc
	clang $(CFLAGS) crop.cc -o crop

denoise: denoise.cc cxxopts/src/cxxopts.hpp
	clang -std=c++11 $(CFLAGS) denoise.cc -o denoise

merge: merge.cc cxxopts/src/cxxopts.hpp
	g++ -std=c++11 $(CFLAGS) merge.cc -o merge

straighten: straighten.cc
	clang $(CFLAGS) straighten.cc -o straighten

unsharp-mask: unsharp-mask.cc cxxopts/src/cxxopts.hpp
	clang -std=c++11 $(CFLAGS) unsharp-mask.cc -o unsharp-mask

