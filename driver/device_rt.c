/**
 * @file device_rt.c
 *
 * @brief Device runtime driver
 *
 * @author Sean James
 */

#include <stdio.h>
#include <cuda.h>
#include <mach-o/getsect.h>
#include <mach-o/dyld.h>
#include <string.h>
#include "device_errors.h"

//#define DEBUG

#ifdef DEBUG

#define checkCudaErrors(expr) {              \
    CUresult err = (expr);                   \
    if (err != CUDA_SUCCESS) {               \
        printf("CUDA Error (line %d): %s\n", \
            __LINE__, getErrorString(err));  \
        exit(-1);                            \
    }                                        \
}

unsigned long long getCycleCount() {
    unsigned int lo, hi;
    __asm__ __volatile__ ("rdtsc" : "=a" (lo), "=d" (hi));
    return ((unsigned long long)hi << 32) | lo;
}

#else

#define checkCudaErrors(expr) (expr);

#endif

int main(int argc, char *argv[]) {
    CUdevice device;
    CUcontext context;
    CUmodule cudaModule;
    CUfunction function;

    checkCudaErrors(cuInit(0));

#ifdef DEBUG
    int devCount;
    checkCudaErrors(cuDeviceGetCount(&devCount));

    if (devCount == 0) {
        printf("No CUDA devices available\n");
        exit(-1);
    }
#endif

    checkCudaErrors(cuDeviceGet(&device, 0));

#ifdef DEBUG
    char name[128];
    int devMajor, devMinor;

    if (argc > 1 && strcmp(argv[1], "--verbose") == 0) {
        checkCudaErrors(cuDeviceGetName(name, 128, device));
        checkCudaErrors(cuDeviceComputeCapability(&devMajor, &devMinor, device));
        printf("Device: %s (Compute %d.%d)\n", name, devMajor, devMinor);
    }
#endif

    checkCudaErrors(cuCtxCreate(&context, 0, device));

    unsigned long size;
    char *kernel_src = getsectdata("__TEXT", "__kernels", &size);

    if (kernel_src == NULL) {
        printf("Error locating kernel\n");
        return -2;
    }

    kernel_src += _dyld_get_image_vmaddr_slide(0);

    // TODO: for now copy the whole thing just because we want to null terminate it. There is
    // probably a nicer way.
    //char *kernel = malloc(size + 1);
    //memcpy(kernel, kernel_src, size);
    //kernel[size] = 0; // null terminate
    char *kernel = kernel_src;

    #define LOG_SZ 1024
    char *errors = malloc(LOG_SZ + LOG_SZ);
    char *info = errors + LOG_SZ;

    int nOptions = 0;
    void *values[16];
    CUjit_option options[16];

    options[nOptions]  = CU_JIT_ERROR_LOG_BUFFER_SIZE_BYTES;
    values[nOptions++] = (void *)LOG_SZ;
    options[nOptions]  = CU_JIT_ERROR_LOG_BUFFER;
    values[nOptions++] = errors;
    options[nOptions]  = CU_JIT_INFO_LOG_BUFFER_SIZE_BYTES;
    values[nOptions++] = (void *)LOG_SZ;
    options[nOptions]  = CU_JIT_INFO_LOG_BUFFER;
    values[nOptions++] = errors;

    CUresult err = cuModuleLoadDataEx(&cudaModule, kernel, nOptions, options, values);
    if (err != CUDA_SUCCESS) {
        printf("JIT failed:%s\n%s\n", info, errors);
        exit(-1);
    }

    free(errors);
    //free(kernel);

    checkCudaErrors(cuModuleGetFunction(&function, cudaModule, "_cc_main"));

    int retVal;
    CUdeviceptr devRetVal;

    checkCudaErrors(cuMemAlloc(&devRetVal, sizeof(int)));

    void *kernelParams[1];
    kernelParams[0] = &devRetVal;

#ifdef DEBUG
    unsigned long long start = getCycleCount();
#endif

    checkCudaErrors(cuLaunchKernel(function, 1, 1, 1, 1, 1, 1, 0, NULL, kernelParams, NULL));

#ifdef DEBUG
    unsigned long long end = getCycleCount();
#endif

    checkCudaErrors(cuMemcpyDtoH(&retVal, devRetVal, sizeof(int)));

    printf("%d\n", retVal);

#ifdef DEBUG
    printf("%llu cycles\n", end - start);
#endif

    checkCudaErrors(cuMemFree(devRetVal));

    checkCudaErrors(cuModuleUnload(cudaModule));
    checkCudaErrors(cuCtxDestroy(context));

    return 0;
}
