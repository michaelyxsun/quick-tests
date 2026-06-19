default:
	nvcc overlap.c -o main -I./libcuest-linux-x86_64-0.1.1.1_cuda13-archive/include -L./libcuest-linux-x86_64-0.1.1.1_cuda13-archive/lib -lcuest
