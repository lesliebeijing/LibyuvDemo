package com.lesliefang.libyuvdemo;

import android.Manifest;
import android.content.pm.PackageManager;
import android.graphics.Bitmap;
import android.graphics.BitmapFactory;
import android.graphics.ImageFormat;
import android.graphics.Rect;
import android.graphics.YuvImage;
import android.hardware.Camera;
import android.os.Bundle;
import android.view.SurfaceHolder;
import android.view.SurfaceView;
import android.widget.Toast;

import androidx.annotation.NonNull;
import androidx.appcompat.app.AppCompatActivity;
import androidx.core.app.ActivityCompat;
import androidx.core.content.ContextCompat;

import com.libyuv.util.YuvUtil;

import java.io.ByteArrayOutputStream;
import java.io.IOException;

public class MainActivity extends AppCompatActivity {
    Camera camera;
    SurfaceView surfaceView;
    BitmapSurfaceView bitmapSurfaceView;
    int dstWith, dstHeight;

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);
        surfaceView = findViewById(R.id.surfaceview);
        bitmapSurfaceView = findViewById(R.id.surfaceview_copy);
        dstWith = getResources().getDimensionPixelSize(R.dimen.video_width);
        dstHeight = getResources().getDimensionPixelSize(R.dimen.video_height);

        if (ContextCompat.checkSelfPermission(this, Manifest.permission.CAMERA) != PackageManager.PERMISSION_GRANTED) {
            ActivityCompat.requestPermissions(this, new String[]{Manifest.permission.CAMERA}, 100);
        } else {
            openCamera();
        }
    }

    private void openCamera() {
        camera = Camera.open(Camera.CameraInfo.CAMERA_FACING_FRONT);
        camera.setDisplayOrientation(90);
        SurfaceHolder holder = surfaceView.getHolder();
        holder.addCallback(new SurfaceHolder.Callback() {
            @Override
            public void surfaceCreated(SurfaceHolder surfaceHolder) {
                try {
                    camera.setPreviewDisplay(surfaceHolder);
                    camera.startPreview();
                    camera.setPreviewCallback(new Camera.PreviewCallback() {
                        @Override
                        public void onPreviewFrame(byte[] bytes, Camera camera) {
                            // bytes 是  NV21 格式的 YUV 数据
                            Camera.Size previewSize = camera.getParameters().getPreviewSize();
                            int width = previewSize.width;
                            int height = previewSize.height;
                            try {
                                // NV21  转 I420
                                byte[] i420Data = new byte[width * height * 3 / 2];
                                YuvUtil.NV21ToI420(bytes, width, height, i420Data);

                                // 镜像
                                byte[] i420MirrorData = new byte[width * height * 3 / 2];
                                YuvUtil.I420Mirror(i420Data, width, height, i420MirrorData);

                                // 缩放，注意缩放后宽高会改变
                                byte[] i420ScaleData = new byte[dstWith * dstHeight * 3 / 2];
                                YuvUtil.I420Scale(i420MirrorData, width, height, i420ScaleData, dstWith, dstHeight, 0);
                                width = dstWith;
                                height = dstHeight;

                                // 旋转： 注意顺时针旋转 90 度后宽高对调了
                                byte[] i420RotateData = new byte[width * height * 3 / 2];
                                YuvUtil.I420Rotate(i420ScaleData, width, height, i420RotateData, 90);
                                int temp = width;
                                width = height;
                                height = temp;

                                // I420 -> NV21
                                byte[] newNV21Data = new byte[width * height * 3 / 2];
                                YuvUtil.I420ToNV21(i420RotateData, width, height, newNV21Data);

                                // 转Bitmap
                                YuvImage yuvImage = new YuvImage(newNV21Data, ImageFormat.NV21, width, height, null);
                                ByteArrayOutputStream stream = new ByteArrayOutputStream();
                                yuvImage.compressToJpeg(new Rect(0, 0, width, height), 80, stream);
                                Bitmap bitmap = BitmapFactory.decodeByteArray(stream.toByteArray(), 0, stream.size());
                                stream.close();

                                if (bitmap != null) {
                                    bitmapSurfaceView.drawBitmap(bitmap);
                                }
                            } catch (IOException e) {
                                e.printStackTrace();
                            }
                        }
                    });
                } catch (IOException e) {
                    e.printStackTrace();
                }
            }

            @Override
            public void surfaceChanged(SurfaceHolder surfaceHolder, int i, int i1, int i2) {

            }

            @Override
            public void surfaceDestroyed(SurfaceHolder surfaceHolder) {
                camera.setPreviewCallback(null);
                camera.stopPreview();
                camera.release();
            }
        });
    }

    @Override
    public void onRequestPermissionsResult(int requestCode, @NonNull String[] permissions, @NonNull int[] grantResults) {
        super.onRequestPermissionsResult(requestCode, permissions, grantResults);
        if (requestCode == 100 && grantResults[0] == PackageManager.PERMISSION_GRANTED) {
            openCamera();
        } else {
            Toast.makeText(this, "需要相机权限", Toast.LENGTH_LONG).show();
        }
    }
}
