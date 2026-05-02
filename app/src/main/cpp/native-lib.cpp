#include <jni.h>
#include <string>
#include <algorithm>
#include <climits>
#include <vector>
#include <cmath>
#include <android/log.h>
#include <sys/ptrace.h>
#include <unistd.h>
#include <cstdio>

#define LOG_TAG "NativeSecurity"
#define LOGI(...) __android_log_print(ANDROID_LOG_INFO, LOG_TAG, __VA_ARGS__)
#define LOGW(...) __android_log_print(ANDROID_LOG_WARN, LOG_TAG, __VA_ARGS__)
#define LOGE(...) __android_log_print(ANDROID_LOG_ERROR, LOG_TAG, __VA_ARGS__)

// Codes d'état (alignés avec NativeSecurityManager.java)
enum SecurityStatus {
    STATUS_OK = 0,
    STATUS_TRACE_DETECTED = 1,
    STATUS_SUSPICIOUS_MAPS = 2,
    STATUS_MULTIPLE_SIGNALS = 3
};

// --- Contrôles Défensifs (Étape 13 - Séparer code métier et défensif) ---

static bool isBeingTraced() {
    if (ptrace(PTRACE_TRACEME, 0, 1, 0) < 0) {
        LOGE("Anti-Debug: ptrace check failed");
        return true;
    }
    return false;
}

static bool hasSuspiciousMaps() {
    FILE* fp = fopen("/proc/self/maps", "r");
    if (!fp) return false;
    char line[512];
    bool found = false;
    while (fgets(line, sizeof(line), fp)) {
        if (strstr(line, "frida") || strstr(line, "xposed") || strstr(line, "magisk")) {
            LOGE("Anti-Debug: Suspicious entry in maps: %s", line);
            found = true;
            break;
        }
    }
    fclose(fp);
    return found;
}

// --- Implémentations JNI ---

jint getSecurityStatusNative(JNIEnv* env, jobject thiz) {
    bool traced = isBeingTraced();
    bool maps = hasSuspiciousMaps();

    if (traced && maps) return STATUS_MULTIPLE_SIGNALS;
    if (traced) return STATUS_TRACE_DETECTED;
    if (maps) return STATUS_SUSPICIOUS_MAPS;

    LOGI("Environment check: OK");
    return STATUS_OK;
}

jstring helloFromJNI(JNIEnv* env, jobject thiz) {
    return env->NewStringUTF("Hello from professional JNI layer!");
}

jint factorial(JNIEnv* env, jobject thiz, jint n) {
    if (n < 0) return -1;
    long long res = 1;
    for (int i = 1; i <= n; ++i) {
        res *= i;
        if (res > INT_MAX) return -2; // Test 3: Overflow
    }
    return (jint)res;
}

jstring reverseString(JNIEnv* env, jobject thiz, jstring jstr) {
    if (!jstr) return env->NewStringUTF("");
    const char* cstr = env->GetStringUTFChars(jstr, nullptr);
    std::string s(cstr);
    env->ReleaseStringUTFChars(jstr, cstr);
    std::reverse(s.begin(), s.end());
    return env->NewStringUTF(s.c_str());
}

jint sumArray(JNIEnv* env, jobject thiz, jintArray jarr) {
    if (!jarr) return 0;
    jsize len = env->GetArrayLength(jarr);
    if (len == 0) return 0;
    jint* elements = env->GetIntArrayElements(jarr, nullptr);
    long long sum = 0;
    for (int i = 0; i < len; ++i) sum += elements[i];
    env->ReleaseIntArrayElements(jarr, elements, JNI_ABORT);
    return (jint)sum;
}

jfloatArray multiplyMatrices(JNIEnv* env, jobject thiz, jfloatArray a, jint r1, jint c1, jfloatArray b, jint r2, jint c2) {
    if (c1 != r2) return nullptr;
    jfloat* dataA = env->GetFloatArrayElements(a, nullptr);
    jfloat* dataB = env->GetFloatArrayElements(b, nullptr);
    int resSize = r1 * c2;
    std::vector<float> res(resSize, 0.0f);
    for (int i = 0; i < r1; ++i)
        for (int j = 0; j < c2; ++j)
            for (int k = 0; k < c1; ++k)
                res[i * c2 + j] += dataA[i * c1 + k] * dataB[k * c2 + j];
    env->ReleaseFloatArrayElements(a, dataA, JNI_ABORT);
    env->ReleaseFloatArrayElements(b, dataB, JNI_ABORT);
    jfloatArray result = env->NewFloatArray(resSize);
    env->SetFloatArrayRegion(result, 0, resSize, res.data());
    return result;
}

jboolean hasForbiddenChars(JNIEnv* env, jobject thiz, jstring str, jstring forbidden) {
    if (!str || !forbidden) return JNI_FALSE;
    const char* cStr = env->GetStringUTFChars(str, nullptr);
    const char* cForbidden = env->GetStringUTFChars(forbidden, nullptr);
    std::string s(cStr), f(cForbidden);
    env->ReleaseStringUTFChars(str, cStr);
    env->ReleaseStringUTFChars(forbidden, cForbidden);
    return s.find_first_of(f) != std::string::npos ? JNI_TRUE : JNI_FALSE;
}

void performHeavyTask(JNIEnv* env, jobject thiz, jint iters) {
    volatile double x = 0;
    for (int i = 0; i < iters; ++i) x += std::sqrt(i);
}

// --- Étape 15 - Variante D : RegisterNatives (Approche Pro) ---

static const JNINativeMethod gMethods[] = {
    {"getSecurityStatusNative", "()I", (void*)getSecurityStatusNative},
    {"helloFromJNI", "()Ljava/lang/String;", (void*)helloFromJNI},
    {"factorial", "(I)I", (void*)factorial},
    {"reverseString", "(Ljava/lang/String;)Ljava/lang/String;", (void*)reverseString},
    {"sumArray", "([I)I", (void*)sumArray},
    {"multiplyMatrices", "([FII[FII)[F", (void*)multiplyMatrices},
    {"hasForbiddenChars", "(Ljava/lang/String;Ljava/lang/String;)Z", (void*)hasForbiddenChars},
    {"performHeavyTask", "(I)V", (void*)performHeavyTask}
};

JNIEXPORT jint JNICALL JNI_OnLoad(JavaVM* vm, void* reserved) {
    JNIEnv* env;
    if (vm->GetEnv((void**)&env, JNI_VERSION_1_6) != JNI_OK) return JNI_ERR;

    jclass clazz = env->FindClass("com/example/jnidemo/NativeSecurityManager");
    if (clazz == nullptr) return JNI_ERR;

    if (env->RegisterNatives(clazz, gMethods, sizeof(gMethods) / sizeof(gMethods[0])) < 0) {
        return JNI_ERR;
    }

    return JNI_VERSION_1_6;
}
