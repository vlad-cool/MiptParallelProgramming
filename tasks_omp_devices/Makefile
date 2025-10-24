# CXX = /usr/local/cuda-13.0/bin/nvcc
# CXX = clang++-18


# CXX = /home/vlad/Downloads/LLVM-21.1.3-Linux-X64/bin/clang++
# CXXFLAGS = -v -O3 -Wall -fopenmp -fopenmp-targets=nvptx64-nvidia-cuda -fopenmp-targets=amdgcn-amd-amdhsa  -L/usr/local/cuda/lib64 -I /usr/lib/clang/20/include -nostdinc++ \

# CXXFLAGS = -v -O3 -Wall -fopenmp -fopenmp-targets=amdgcn-amd-amdhsa  -L/usr/local/cuda/lib64 -I /usr/lib/clang/20/include -nostdinc++ --rocm-path=/usr/lib64/rocm/llvm/lib/clang/18/amdgcn/bitcode

# CXX = /usr/local/cuda/bin/nvcc
# CXXFLAGS = -gpu=cc80
# CXX = /usr/lib64/rocm/llvm/bin/clang++
CXX = g++
CXXFLAGS = -fopenmp -foffload=nvptx-none -O3
# CXXFLAGS = -fopenmp -fopenmp-targets=amdgcn-amd-amdhsa
# 


# CXX = /home/vlad/Storage/LLVM/llvm-install/bin/clang++
# CXXFLAGS = -fopenmp --offload=nvptx-none -ffast-math -fno-lto -fopenmp-targets=nvptx64-nvidia-cuda
# CXXFLAGS = -v -ffast-math -fno-lto -O3 -Wall -fopenmp -fopenmp-targets=nvptx64-nvidia-cuda --cuda-path=/usr/local/cuda -fopenmp-version=52

# CXXFLAGS = -v -O3 -Wall -fopenmp -fopenmp-targets=nvptx64-nvidia-cuda --cuda-path=/usr/local/cuda -L/usr/local/cuda/lib64 -I /usr/lib/clang/20/include -nostdinc++ \
#   -isystem /usr/include/c++/15 \
#   -isystem /usr/include/c++/15/x86_64-redhat-linux \
#   -isystem /usr/include \-nostdinc++ \
#   -isystem /usr/include/c++/15 \
#   -isystem /usr/include/c++/15/x86_64-redhat-linux \
#   -isystem /usr/include
SOURCES = $(wildcard src/*.cpp)
TARGETS = $(addprefix bin/, $(SOURCES:src/%.cpp=%))

.PHONY: clean all

all: $(TARGETS)

bin/%: src/%.cpp
	mkdir -p bin
	$(CXX) $(CXXFLAGS) -o $@ $<

bin:
	mkdir bin

clean:
	rm -f $(TARGETS)
