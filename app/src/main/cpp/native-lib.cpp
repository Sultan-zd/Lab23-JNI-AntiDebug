#include <jni.h>
#include <string>
#include <algorithm>
#include <climits>
#include <vector>
#include <android/log.h>

#define LOG_TAG "JNI_DEMO"
#define LOGI(...) __android_log_print(ANDROID_LOG_INFO, LOG_TAG, __VA_ARGS__)

// --- Méthodes existantes (gardées pour la compatibilité ou ré-enregistrées) ---

jstring helloFromJNI(JNIEnv* env, jobject thiz) {
    return env->NewStringUTF("Hello from C++ via RegisterNatives !");
}

jint factorial(JNIEnv* env, jobject thiz, jint n) {
    if (n < 0) return -1;
    long long fact = 1;
    for (int i = 1; i <= n; i++) {
        fact *= i;
        if (fact > INT_MAX) return -2;
    }
    return static_cast<jint>(fact);
}

jstring reverseString(JNIEnv* env, jobject thiz, jstring javaString) {
    if (javaString == nullptr) return env->NewStringUTF("");
    const char* chars = env->GetStringUTFChars(javaString, nullptr);
    std::string s(chars);
    env->ReleaseStringUTFChars(javaString, chars);
    std::reverse(s.begin(), s.end());
    return env->NewStringUTF(s.c_str());
}

jint sumArray(JNIEnv* env, jobject thiz, jintArray array) {
    if (array == nullptr) return 0;
    jsize len = env->GetArrayLength(array);
    jint* elements = env->GetIntArrayElements(array, nullptr);
    long long sum = 0;
    for (jsize i = 0; i < len; i++) sum += elements[i];
    env->ReleaseIntArrayElements(array, elements, 0);
    return static_cast<jint>(sum);
}

// --- Extension A : Multiplication Matricielle ---

jfloatArray multiplyMatrices(JNIEnv* env, jobject thiz,
                             jfloatArray a, jint r1, jint c1,
                             jfloatArray b, jint r2, jint c2) {
    if (c1 != r2) return nullptr;

    jfloat* dataA = env->GetFloatArrayElements(a, nullptr);
    jfloat* dataB = env->GetFloatArrayElements(b, nullptr);

    int resSize = r1 * c2;
    std::vector<float> res(resSize, 0.0f);

    for (int i = 0; i < r1; ++i) {
        for (int j = 0; j < c2; ++j) {
            for (int k = 0; k < c1; ++k) {
                res[i * c2 + j] += dataA[i * c1 + k] * dataB[k * c2 + j];
            }
        }
    }

    env->ReleaseFloatArrayElements(a, dataA, JNI_ABORT);
    env->ReleaseFloatArrayElements(b, dataB, JNI_ABORT);

    jfloatArray result = env->NewFloatArray(resSize);
    env->SetFloatArrayRegion(result, 0, resSize, res.data());
    return result;
}

// --- Extension B : Détection de caractères interdits ---

jboolean hasForbiddenChars(JNIEnv* env, jobject thiz, jstring str, jstring forbidden) {
    if (str == nullptr || forbidden == nullptr) return JNI_FALSE;

    const char* cStr = env->GetStringUTFChars(str, nullptr);
    const char* cForbidden = env->GetStringUTFChars(forbidden, nullptr);

    std::string s(cStr);
    std::string f(cForbidden);

    env->ReleaseStringUTFChars(str, cStr);
    env->ReleaseStringUTFChars(forbidden, cForbidden);

    for (char c : s) {
        if (f.find(c) != std::string::npos) return JNI_TRUE;
    }
    return JNI_FALSE;
}

// --- Extension C : Aide au Benchmark ---

void performHeavyTask(JNIEnv* env, jobject thiz, jint iterations) {
    volatile double result = 0.0;
    for (int i = 0; i < iterations; i++) {
        result += std::sqrt(static_cast<double>(i));
    }
}

// --- Extension D : RegisterNatives ---

static const JNINativeMethod gMethods[] = {
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
    if (vm->GetEnv((void**)&env, JNI_VERSION_1_6) != JNI_OK) {
        return JNI_ERR;
    }

    jclass clazz = env->FindClass("com/example/jnidemo/MainActivity");
    if (clazz == nullptr) {
        return JNI_ERR;
    }

    if (env->RegisterNatives(clazz, gMethods, sizeof(gMethods) / sizeof(gMethods[0])) < 0) {
        return JNI_ERR;
    }

    return JNI_VERSION_1_6;
}
