package com.lesliefang.libyuvdemo;

import android.content.Context;
import android.graphics.Bitmap;
import android.graphics.Canvas;
import android.graphics.Paint;
import android.util.AttributeSet;
import android.view.SurfaceHolder;
import android.view.SurfaceView;

/**
 * @创建者 LQR
 * @时间 2019/9/18
 * @描述 专门绘制Bitmap的SurfaceView
 */
public class BitmapSurfaceView extends SurfaceView implements SurfaceHolder.Callback, Runnable {

    private SurfaceHolder mHolder;
    private Thread mThread;
    private boolean mIsDrawing;
    private Bitmap mBitmap;
    private Paint mPaint;

    public BitmapSurfaceView(Context context) {
        this(context, null);
    }

    public BitmapSurfaceView(Context context, AttributeSet attrs) {
        this(context, attrs, 0);
    }

    public BitmapSurfaceView(Context context, AttributeSet attrs, int defStyleAttr) {
        super(context, attrs, defStyleAttr);
        mHolder = getHolder();
        mHolder.addCallback(this);
        mPaint = new Paint();
    }

    @Override
    public void surfaceCreated(SurfaceHolder holder) {
        mThread = new Thread(this);
        mThread.start();
        mIsDrawing = true;
    }

    @Override
    public void surfaceChanged(SurfaceHolder holder, int format, int width, int height) {
    }

    @Override
    public void surfaceDestroyed(SurfaceHolder holder) {
        mIsDrawing = false;
    }

    @Override
    public void run() {
        while (mIsDrawing) {
            try {
                if (mHolder != null && mBitmap != null) {
                    Canvas canvas = mHolder.lockCanvas();
                    canvas.drawBitmap(mBitmap, 0, 0, mPaint);
                    mHolder.unlockCanvasAndPost(canvas);
                    Thread.sleep(10);
                }
            } catch (Exception e) {
                e.printStackTrace();
            }
        }
    }

    public void drawBitmap(final Bitmap bitmap) {
        post(new Runnable() {
            @Override
            public void run() {
                mBitmap = bitmap;
            }
        });
    }
}