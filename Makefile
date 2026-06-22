default:
	nvcc overlap.c -o main -I./libcuest-linux-x86_64-0.1.1.1_cuda13-archive/include -L./libcuest-linux-x86_64-0.1.1.1_cuda13-archive/lib -lcuest

f:
	gfortran -c overlap.f90
	nvcc -c overlap_f90_call.c -I./libcuest-linux-x86_64-0.1.1.1_cuda13-archive/include -L./libcuest-linux-x86_64-0.1.1.1_cuda13-archive/lib -lcuest
	gfortran overlap.o overlap_f90_call.o -o main -L/usr/local/cuda/lib64 -lcudart

clean:
	rm *.o
