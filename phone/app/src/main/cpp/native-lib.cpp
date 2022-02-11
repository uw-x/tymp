#include <jni.h>
#include <string.h>
#include <fftw3.h>
#include <math.h>

extern "C" JNIEXPORT jdoubleArray JNICALL Java_com_example_root_tymp_MainActivity_fftnativetrim(
        JNIEnv *env,
        jobject,
        jdoubleArray data,
        jint N,
        jint outcropstart,
        jint outcropend) {

    fftw_complex *in , *out;
    fftw_plan p;

    in = (fftw_complex*) fftw_malloc(sizeof(fftw_complex) * N);
    out = (fftw_complex*) fftw_malloc(sizeof(fftw_complex) * N);

    for (int i = 0; i < N; i++) {
        in[i][0] = 0;
        in[i][1] = 0;
        out[i][0] = 0;
        out[i][1] = 0;
    }

    jdouble *doubleArray = env->GetDoubleArrayElements(data, NULL);
    int datalen = env -> GetArrayLength(data);
    for (int i = 0; i < datalen; i++) {
        in[i][0] = doubleArray[i];
    }

    p = fftw_plan_dft_1d(N, in, out, FFTW_FORWARD, FFTW_ESTIMATE);
    fftw_execute(p);

    jint outlen = outcropend-outcropstart;
    jdouble mag[outlen];
    int counter=0;
//    for (int i = 0; i < N; i++) {
    for (int i = outcropstart-1; i < outcropend-1; i++) {
        double real = out[i][0];
        double imag = out[i][1];

        mag[counter++] = sqrt((real*real)+(imag*imag));
    }

    jdoubleArray result;
    result = env->NewDoubleArray(outlen);
    env->SetDoubleArrayRegion(result, 0, outlen, mag);

    fftw_destroy_plan(p);
    fftw_free(in); fftw_free(out);

    return result;
}

extern "C" JNIEXPORT jdoubleArray JNICALL Java_com_example_root_tymp_DataProc_fftnative(
        JNIEnv *env,
        jclass,
        jintArray data,
        jint N) {

    fftw_complex *in , *out;
    fftw_plan p;

    in = (fftw_complex*) fftw_malloc(sizeof(fftw_complex) * N);
    out = (fftw_complex*) fftw_malloc(sizeof(fftw_complex) * N);

    for (int i = 0; i < N; i++) {
        in[i][0] = 0;
        in[i][1] = 0;
        out[i][0] = 0;
        out[i][1] = 0;
    }

    jint *intArray = env->GetIntArrayElements(data, NULL);
    int datalen = env -> GetArrayLength(data);
    for (int i = 0; i < datalen; i++) {
        in[i][0] = intArray[i];
    }

    p = fftw_plan_dft_1d(N, in, out, FFTW_FORWARD, FFTW_ESTIMATE);
    fftw_execute(p);

    jdouble mag[N];
    int counter=0;
    for (int i = 0; i < N; i++) {
        double real = out[i][0];
        double imag = out[i][1];

        mag[i] = sqrt((real*real)+(imag*imag));
    }

    jdoubleArray result;
    result = env->NewDoubleArray(N);
    env->SetDoubleArrayRegion(result, 0, N, mag);

    fftw_destroy_plan(p);
    fftw_free(in); fftw_free(out);

    return result;
}

extern "C" JNIEXPORT jobjectArray JNICALL Java_com_example_root_tymp_MainActivity_fftcomplexoutnative(
        JNIEnv *env,
        jobject,
        jdoubleArray data,
        jint N) {

    fftw_complex *in , *out;
    fftw_plan p;

    in = (fftw_complex*) fftw_malloc(sizeof(fftw_complex) * N);
    out = (fftw_complex*) fftw_malloc(sizeof(fftw_complex) * N);

    for (int i = 0; i < N; i++) {
        in[i][0] = 0;
        in[i][1] = 0;
        out[i][0] = 0;
        out[i][1] = 0;
    }

    jdouble *doubleArray = env->GetDoubleArrayElements(data, NULL);
    int datalen = env -> GetArrayLength(data);
    for (int i = 0; i < datalen; i++) {
        in[i][0] = doubleArray[i];
    }

    p = fftw_plan_dft_1d(N, in, out, FFTW_FORWARD, FFTW_ESTIMATE);
    fftw_execute(p);

    jdouble real[N];
    jdouble imag[N];
    for (int i = 0; i < N; i++) {
        real[i] = out[i][0];
        imag[i] = out[i][1];
    }

    jdoubleArray realResult;
    jdoubleArray imagResult;
    realResult = env->NewDoubleArray(N);
    imagResult = env->NewDoubleArray(N);
    env->SetDoubleArrayRegion(realResult, 0, N, real);
    env->SetDoubleArrayRegion(imagResult, 0, N, imag);

    jobjectArray outarray = env->NewObjectArray(2, env->GetObjectClass(realResult), 0);
    env->SetObjectArrayElement(outarray, 0, realResult);
    env->SetObjectArrayElement(outarray, 1, imagResult);

    fftw_destroy_plan(p);
    fftw_free(in); fftw_free(out);

    return outarray;
}

extern "C" JNIEXPORT jdoubleArray JNICALL Java_com_example_root_tymp_MainActivity_ifftnative(
        JNIEnv *env,
        jobject,
        jobjectArray data) {

    jdoubleArray real = (jdoubleArray) env->GetObjectArrayElement(data, 0);
    jdoubleArray imag = (jdoubleArray) env->GetObjectArrayElement(data, 1);

    jint N = env -> GetArrayLength(real);

    fftw_complex *in , *out;
    fftw_plan p;

    in = (fftw_complex*) fftw_malloc(sizeof(fftw_complex) * N);
    out = (fftw_complex*) fftw_malloc(sizeof(fftw_complex) * N);

    for (int i = 0; i < N; i++) {
        in[i][0] = 0;
        in[i][1] = 0;
        out[i][0] = 0;
        out[i][1] = 0;
    }

    jdouble *realArray = env->GetDoubleArrayElements(real, NULL);
    jdouble *imagArray = env->GetDoubleArrayElements(imag, NULL);
    for (int i = 0; i < N; i++) {
        in[i][0] = realArray[i];
    }
    for (int i = 0; i < N; i++) {
        in[i][1] = imagArray[i];
    }

    p = fftw_plan_dft_1d(N, in, out, FFTW_BACKWARD, FFTW_ESTIMATE);
    fftw_execute(p);

    jdouble realout[N/2];
    int counter = 0;
    for (int i = 0; i < N; i+=2) {
        realout[counter++] = out[i][0];
    }

    jdoubleArray result;
    result = env->NewDoubleArray(N/2);
    env->SetDoubleArrayRegion(result, 0, N/2, realout);

    fftw_destroy_plan(p);
    fftw_free(in); fftw_free(out);

    return result;
}


extern "C" JNIEXPORT void JNICALL Java_com_example_root_tymp_MainActivity_conjnative(
        JNIEnv *env,
        jobject,
        jobjectArray data) {

    jdoubleArray imag = (jdoubleArray) env->GetObjectArrayElement(data, 1);
    jdouble *imagArray = env->GetDoubleArrayElements(imag, NULL);
    jint N = env -> GetArrayLength(imag);

    for (int i = 0; i < N; i++) {
        imagArray[i] = -imagArray[i];
    }
}

extern "C" JNIEXPORT jobjectArray JNICALL Java_com_example_root_tymp_MainActivity_timesnative(
        JNIEnv *env,
        jobject,
        jobjectArray c1,
        jobjectArray c2) {

    jdoubleArray realar1 = (jdoubleArray) env->GetObjectArrayElement(c1, 0);
    jdoubleArray imagar1 = (jdoubleArray) env->GetObjectArrayElement(c1, 1);
    jdoubleArray realar2 = (jdoubleArray) env->GetObjectArrayElement(c2, 0);
    jdoubleArray imagar2 = (jdoubleArray) env->GetObjectArrayElement(c2, 1);

    jdouble *real1 = env->GetDoubleArrayElements(realar1, NULL);
    jdouble *imag1 = env->GetDoubleArrayElements(imagar1, NULL);
    jdouble *real2 = env->GetDoubleArrayElements(realar2, NULL);
    jdouble *imag2 = env->GetDoubleArrayElements(imagar2, NULL);

    jint N = env -> GetArrayLength(realar1);

    jint counter = 0;
    jdouble real[N];
    jdouble imag[N];
    for (int i = 0; i < N; i++) {
        real[counter] = real1[i]*real2[i]-imag1[i]*imag2[i];
        imag[counter++] = imag1[i]*real2[i]+real1[i]*imag2[i];
    }

    jdoubleArray realResult;
    jdoubleArray imagResult;
    realResult = env->NewDoubleArray(N);
    imagResult = env->NewDoubleArray(N);
    env->SetDoubleArrayRegion(realResult, 0, N, real);
    env->SetDoubleArrayRegion(imagResult, 0, N, imag);

    jobjectArray outarray = env->NewObjectArray(2, env->GetObjectClass(realResult), 0);
    env->SetObjectArrayElement(outarray, 0, realResult);
    env->SetObjectArrayElement(outarray, 1, imagResult);

    return outarray;
}
