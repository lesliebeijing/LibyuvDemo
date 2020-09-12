//
// Created by fangleslie on 2020/9/10.
//
#include <jni.h>
#include "libyuv.h"

/**
 * NV21 -> I420
 */
extern "C"
JNIEXPORT void JNICALL
Java_com_libyuv_util_YuvUtil_NV21ToI420(JNIEnv *env, jclass clazz, jbyteArray src_nv21_array,
                                        jint width, jint height, jbyteArray dst_i420_array) {
    jbyte *src_nv21_data = env->GetByteArrayElements(src_nv21_array, JNI_FALSE);
    jbyte *dst_i420_data = env->GetByteArrayElements(dst_i420_array, JNI_FALSE);

    jint src_y_size = width * height;
    jint src_u_size = (width >> 1) * (height >> 1);

    jbyte *src_nv21_y_data = src_nv21_data;
    jbyte *src_nv21_vu_data = src_nv21_data + src_y_size;

    jbyte *dst_i420_y_data = dst_i420_data;
    jbyte *dst_i420_u_data = dst_i420_data + src_y_size;
    jbyte *dst_i420_v_data = dst_i420_data + src_y_size + src_u_size;

    libyuv::NV21ToI420((const uint8_t *) src_nv21_y_data, width,
                       (const uint8_t *) src_nv21_vu_data, width,
                       (uint8_t *) dst_i420_y_data, width,
                       (uint8_t *) dst_i420_u_data, width >> 1,
                       (uint8_t *) dst_i420_v_data, width >> 1,
                       width, height);

    env->ReleaseByteArrayElements(src_nv21_array, src_nv21_data, 0);
    env->ReleaseByteArrayElements(dst_i420_array, dst_i420_data, 0);

}

/**
 * I420 -> NV21
 */
extern "C"
JNIEXPORT void JNICALL
Java_com_libyuv_util_YuvUtil_I420ToNV21(JNIEnv *env, jclass clazz, jbyteArray src_i420_array,
                                        jint width, jint height, jbyteArray dst_nv21_array) {
    jbyte *src_i420_data = env->GetByteArrayElements(src_i420_array, JNI_FALSE);
    jbyte *dst_nv21_data = env->GetByteArrayElements(dst_nv21_array, JNI_FALSE);

    jint src_y_size = width * height;
    jint src_u_size = (width >> 1) * (height >> 1);

    jbyte *src_i420_y_data = src_i420_data;
    jbyte *src_i420_u_data = src_i420_data + src_y_size;
    jbyte *src_i420_v_data = src_i420_data + src_y_size + src_u_size;

    jbyte *dst_nv21_y_data = dst_nv21_data;
    jbyte *dst_nv21_uv_data = dst_nv21_data + src_y_size;

    libyuv::I420ToNV21(
            (const uint8_t *) src_i420_y_data, width,
            (const uint8_t *) src_i420_u_data, width >> 1,
            (const uint8_t *) src_i420_v_data, width >> 1,
            (uint8_t *) dst_nv21_y_data, width,
            (uint8_t *) dst_nv21_uv_data, width,
            width, height);

    env->ReleaseByteArrayElements(src_i420_array, src_i420_data, 0);
    env->ReleaseByteArrayElements(dst_nv21_array, dst_nv21_data, 0);
}

/**
 * I420旋转
 */
extern "C"
JNIEXPORT void JNICALL
Java_com_libyuv_util_YuvUtil_I420Rotate(JNIEnv *env, jclass clazz, jbyteArray src_i420_array,
                                        jint width, jint height, jbyteArray dst_i420_array,
                                        jint degree) {
    jbyte *src_i420_data = env->GetByteArrayElements(src_i420_array, JNI_FALSE);
    jbyte *dst_i420_data = env->GetByteArrayElements(dst_i420_array, JNI_FALSE);

    jint src_i420_y_size = width * height;
    jint src_i420_u_size = (width >> 1) * (height >> 1);

    jbyte *src_i420_y_data = src_i420_data;
    jbyte *src_i420_u_data = src_i420_data + src_i420_y_size;
    jbyte *src_i420_v_data = src_i420_data + src_i420_y_size + src_i420_u_size;

    jbyte *dst_i420_y_data = dst_i420_data;
    jbyte *dst_i420_u_data = dst_i420_data + src_i420_y_size;
    jbyte *dst_i420_v_data = dst_i420_data + src_i420_y_size + src_i420_u_size;

    if (degree == libyuv::kRotate90 || degree == libyuv::kRotate270) {
        // 选择 90或 270 的width和height在旋转之后是相反的
        libyuv::I420Rotate((const uint8_t *) src_i420_y_data, width,
                           (const uint8_t *) src_i420_u_data, width >> 1,
                           (const uint8_t *) src_i420_v_data, width >> 1,
                           (uint8_t *) dst_i420_y_data, height,
                           (uint8_t *) dst_i420_u_data, height >> 1,
                           (uint8_t *) dst_i420_v_data, height >> 1,
                           width, height,
                           (libyuv::RotationMode) degree);
    } else {
        libyuv::I420Rotate((const uint8_t *) src_i420_y_data, width,
                           (const uint8_t *) src_i420_u_data, width >> 1,
                           (const uint8_t *) src_i420_v_data, width >> 1,
                           (uint8_t *) dst_i420_y_data, width,
                           (uint8_t *) dst_i420_u_data, width >> 1,
                           (uint8_t *) dst_i420_v_data, width >> 1,
                           width, height,
                           (libyuv::RotationMode) degree);
    }

    env->ReleaseByteArrayElements(src_i420_array, src_i420_data, 0);
    env->ReleaseByteArrayElements(dst_i420_array, dst_i420_data, 0);
}

/**
 * I420镜像
 */
extern "C"
JNIEXPORT void JNICALL
Java_com_libyuv_util_YuvUtil_I420Mirror(JNIEnv *env, jclass type, jbyteArray src_i420_array,
                                        jint width, jint height, jbyteArray dst_i420_array) {
    jbyte *src_i420_data = env->GetByteArrayElements(src_i420_array, JNI_FALSE);
    jbyte *dst_i420_data = env->GetByteArrayElements(dst_i420_array, JNI_FALSE);

    jint src_i420_y_size = width * height;
    jint src_i420_u_size = src_i420_y_size >> 2;

    jbyte *src_i420_y_data = src_i420_data;
    jbyte *src_i420_u_data = src_i420_data + src_i420_y_size;
    jbyte *src_i420_v_data = src_i420_data + src_i420_y_size + src_i420_u_size;

    jbyte *dst_i420_y_data = dst_i420_data;
    jbyte *dst_i420_u_data = dst_i420_data + src_i420_y_size;
    jbyte *dst_i420_v_data = dst_i420_data + src_i420_y_size + src_i420_u_size;

    libyuv::I420Mirror((const uint8_t *) src_i420_y_data, width,
                       (const uint8_t *) src_i420_u_data, width >> 1,
                       (const uint8_t *) src_i420_v_data, width >> 1,
                       (uint8_t *) dst_i420_y_data, width,
                       (uint8_t *) dst_i420_u_data, width >> 1,
                       (uint8_t *) dst_i420_v_data, width >> 1,
                       width, height);

    env->ReleaseByteArrayElements(src_i420_array, src_i420_data, 0);
    env->ReleaseByteArrayElements(dst_i420_array, dst_i420_data, 0);
}

/**
 * I420缩放
 */
extern "C"
JNIEXPORT void JNICALL
Java_com_libyuv_util_YuvUtil_I420Scale(JNIEnv *env, jclass type, jbyteArray src_i420_array,
                                       jint width, jint height, jbyteArray dst_i420_array,
                                       jint dst_width, jint dst_height, jint mode) {
    jbyte *src_i420_data = env->GetByteArrayElements(src_i420_array, JNI_FALSE);
    jbyte *dst_i420_data = env->GetByteArrayElements(dst_i420_array, JNI_FALSE);

    jint src_i420_y_size = width * height;
    jint src_i420_u_size = (width >> 1) * (height >> 1);
    jbyte *src_i420_y_data = src_i420_data;
    jbyte *src_i420_u_data = src_i420_data + src_i420_y_size;
    jbyte *src_i420_v_data = src_i420_data + src_i420_y_size + src_i420_u_size;

    jint dst_i420_y_size = dst_width * dst_height;
    jint dst_i420_u_size = (dst_width >> 1) * (dst_height >> 1);
    jbyte *dst_i420_y_data = dst_i420_data;
    jbyte *dst_i420_u_data = dst_i420_data + dst_i420_y_size;
    jbyte *dst_i420_v_data = dst_i420_data + dst_i420_y_size + dst_i420_u_size;

    libyuv::I420Scale((const uint8_t *) src_i420_y_data, width,
                      (const uint8_t *) src_i420_u_data, width >> 1,
                      (const uint8_t *) src_i420_v_data, width >> 1,
                      width, height,
                      (uint8_t *) dst_i420_y_data, dst_width,
                      (uint8_t *) dst_i420_u_data, dst_width >> 1,
                      (uint8_t *) dst_i420_v_data, dst_width >> 1,
                      dst_width, dst_height,
                      (libyuv::FilterMode) mode);

    env->ReleaseByteArrayElements(src_i420_array, src_i420_data, 0);
    env->ReleaseByteArrayElements(dst_i420_array, dst_i420_data, 0);
}
