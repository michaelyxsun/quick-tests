default:
	export LD_LIBRARY_PATH="./libcuest-linux-x86_64-0.1.1.1_cuda13-archive/lib:$LD_LBRARY_PATH"
	nvcc overlap.c -o main -I./libcuest-linux-x86_64-0.1.1.1_cuda13-archive/include -L./libcuest-linux-x86_64-0.1.1.1_cuda13-archive/lib -lcuest
