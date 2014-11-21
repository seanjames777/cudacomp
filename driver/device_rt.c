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

#define checkCudaErrors(expr) {        \
    CUresult err = (expr);             \
    if (err != CUDA_SUCCESS) {         \
        printf("CUDA Error (line %d)", \
            __LINE__);                 \
        exit(-1);                      \
    }                                  \
}

int main(int argc, char *argv[]) {
    CUdevice device;
    CUcontext context;
    CUmodule cudaModule;
    CUfunction function;

    checkCudaErrors(cuInit(0));

    int devCount;
    checkCudaErrors(cuDeviceGetCount(&devCount));

    checkCudaErrors(cuDeviceGet(&device, 0));

    char name[128];
    int devMajor, devMinor;
    checkCudaErrors(cuDeviceGetName(name, 128, device));
    checkCudaErrors(cuDeviceComputeCapability(&devMajor, &devMinor, device));

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
    char *kernel = malloc(size + 1);
    memcpy(kernel, kernel_src, size);
    kernel[size] = 0; // null terminate

    #define LOG_SZ 1024
    char *errors = malloc(LOG_SZ);

    int nOptions = 0;
    void *values[16];
    CUjit_option options[16];

    options[nOptions]  = CU_JIT_ERROR_LOG_BUFFER_SIZE_BYTES;
    values[nOptions++] = (void *)LOG_SZ;
    options[nOptions]  = CU_JIT_ERROR_LOG_BUFFER;
    values[nOptions++] = errors;

    CUresult err = cuModuleLoadDataEx(&cudaModule, kernel, nOptions, options, values);
    if (err != CUDA_SUCCESS)
        printf("%s\n", errors);
    checkCudaErrors(err);

    free(errors);
    free(kernel);

    checkCudaErrors(cuModuleGetFunction(&function, cudaModule, "_cc_main"));

    int retVal;
    CUdeviceptr devRetVal;

    checkCudaErrors(cuMemAlloc(&devRetVal, sizeof(int)));

    void *kernelParams[1];
    kernelParams[0] = &devRetVal;

    checkCudaErrors(cuLaunchKernel(function, 1, 1, 1, 1, 1, 1, 0, NULL, kernelParams, NULL));

    checkCudaErrors(cuMemcpyDtoH(&retVal, devRetVal, sizeof(int)));

    printf("%d\n", retVal);

    checkCudaErrors(cuMemFree(devRetVal));

    checkCudaErrors(cuModuleUnload(cudaModule));
    checkCudaErrors(cuCtxDestroy(context));

    return 0;
}
