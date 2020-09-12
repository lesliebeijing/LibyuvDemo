package com.libyuv.util;

/**
 * Created by fanglin on 2020/9/10.
 */
public class YuvUtil {

    static {
        System.loadLibrary("yuvutil");
    }

    /**
     * NV21 -> I420
     *
     * @param src_nv21_data 原始NV21数据
     * @param width         原始宽
     * @param height        原始高
     * @param dst_i420_data 目前I420数据
     */
    public static native void NV21ToI420(byte[] src_nv21_data, int width, int height, byte[] dst_i420_data);

    /**
     * I420 -> NV21
     *
     * @param src_i420_data
     * @param width
     * @param height
     * @param dst_nv21_data
     */
    public static native void I420ToNV21(byte[] src_i420_data, int width, int height, byte[] dst_nv21_data);

    /**
     * I420旋转
     *
     * @param src_i420_data 原始数据
     * @param width         原始宽
     * @param height        原始高
     * @param dst_i420_data 目标数据
     * @param degree        旋转角度
     */
    public static native void I420Rotate(byte[] src_i420_data, int width, int height, byte[] dst_i420_data, int degree);

    /**
     * I420镜像
     */
    public static native void I420Mirror(byte[] src_i420_data, int width, int height, byte[] dst_i420_data);

    /**
     * I420缩放
     *
     * @param src_i420_data 原始数据
     * @param width         原始宽
     * @param height        原始高
     * @param dst_i420_data 目标数据
     * @param dst_width     目标宽
     * @param dst_height    目标高
     * @param filter_mode   一般传 0，速度最快，参考源码
     */
    public static native void I420Scale(byte[] src_i420_data, int width, int height, byte[] dst_i420_data, int dst_width, int dst_height, int filter_mode);

}

