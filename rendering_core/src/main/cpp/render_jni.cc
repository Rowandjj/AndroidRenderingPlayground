//
// Created by jj Rowand on 1/16/21.
//

#include "render_factory.h"
#include "jni.h"
#include "log.h"
#include <android/native_window.h>
#include <android/native_window_jni.h>
#include <memory>

#define LOG_TAG "RENDER_JNI"
using namespace bw;

typedef union {
    JNIEnv* env;
    void* venv;
} UnionJNIEnvToVoid;

#define HOLDER \
    reinterpret_cast<RenderInterface*>(handle)

static jlong NativeCreate(JNIEnv *env, jobject thiz, jboolean async) {
    ALOGI("NativeCreate Called");
    auto interface = RenderFactory::CreateRenderInterface(async == JNI_TRUE);
    return reinterpret_cast<jlong>(interface.release());
}

static void NativeDestroy(JNIEnv *env, jobject thiz, jlong handle) {
    ALOGI("NativeDestroy Called");
    delete HOLDER;
}

static void NativeSurfaceCreated(JNIEnv *env,jobject thiz,jlong handle,jobject surface,jint width, jint height) {
    ALOGI("NativeSurfaceCreated Called");
    HOLDER->AttachSurface(ANativeWindow_fromSurface(env, surface), width, height);
}

static void NativeSurfaceDestroyed(JNIEnv *env,jobject thiz,jlong handle) {
    ALOGI("NativeSurfaceDestroyed Called");
    HOLDER->DetachSurface();
}

static void NativeSurfaceSizeChanged(JNIEnv *env,jobject thiz,jlong handle,jint width,jint height) {
    ALOGI("NativeSurfaceSizeChanged Called");
    HOLDER->ChangeSurfaceSize(width, height);
}

static void NativeDrawFrame(JNIEnv *env, jobject thiz,jlong handle, jlong frame_in_nanos) {
    ALOGI("NativeDrawFrame Called");
    HOLDER->DrawFrame(frame_in_nanos);
}

static void NativeRenderPause(JNIEnv *env, jobject thiz, jlong handle) {
    // TODO: implement nativeRenderPause()
    ALOGI("NativeRenderPause Called");

}

static void NativeRenderResume(JNIEnv *env, jobject thiz, jlong handle) {
    // TODO: implement nativeRenderResume()
    ALOGI("NativeRenderResume Called");

}

///////////////////////// JNI 注册 /////////////////////////

static int RegisterNativeMethods(JNIEnv* env, const char* className,
                                 JNINativeMethod* gMethods, int numMethods) {
    jclass target = env->FindClass(className);
    if(target == nullptr) {
        ALOGE("Native registration unable to find class '%s'", className);
        return JNI_FALSE;
    }
    if(env->RegisterNatives(target, gMethods, numMethods) < 0) {
        ALOGE("RegisterNatives failed for '%s'", className);
        return JNI_FALSE;
    }
    return JNI_TRUE;
}

static int RegisterNatives(JNIEnv* env) {
    static const char* clz_name = "com/github/android/rendering/core/GLRenderInterface";
    static JNINativeMethod methods[] = {
            {"nativeCreate", "(Z)J",(void*)NativeCreate},
            {"nativeDestroy", "(J)V",(void*)NativeDestroy},
            {"nativeSurfaceCreated", "(JLandroid/view/Surface;II)V",(void*)NativeSurfaceCreated},
            {"nativeSurfaceSizeChanged", "(JII)V",(void*)NativeSurfaceSizeChanged},
            {"nativeSurfaceDestroyed", "(J)V",(void*)NativeSurfaceDestroyed},
            {"nativeRenderPause", "(J)V",(void*)NativeRenderPause},
            {"nativeRenderResume", "(J)V",(void*)NativeRenderResume},
            {"nativeDrawFrame", "(JJ)V",(void*)NativeDrawFrame},
    };
    int numMethods = sizeof(methods) / sizeof(methods[0]);

    if(!RegisterNativeMethods(env, clz_name, methods, numMethods)) {
        return JNI_FALSE;
    }
    return JNI_TRUE;
}

jint JNI_OnLoad(JavaVM* vm, void* /*reserved*/) {
    UnionJNIEnvToVoid uenv;
    uenv.venv = nullptr;
    JNIEnv* env;

    jint result = -1;

    if(vm->GetEnv(&uenv.venv, JNI_VERSION_1_4) != JNI_OK) {
        ALOGE("JNI GetENV failed");
        return result;
    }

    env = uenv.env;
    if(!RegisterNatives(env)) {
        ALOGE("JNI Register Native Methods failed");
        return result;
    }

    result = JNI_VERSION_1_4;
    return result;
}