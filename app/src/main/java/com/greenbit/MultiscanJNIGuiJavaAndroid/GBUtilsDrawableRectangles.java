package com.greenbit.MultiscanJNIGuiJavaAndroid;

import android.graphics.Paint;
import android.graphics.Canvas;

public class GBUtilsDrawableRectangles {
    public int Left,Top,Bottom,Right;
    public int RectangleColor;
    public boolean IsReadyForDraw;
    public int BmpImgSx, BmpImgSy;

    public GBUtilsDrawableRectangles(int l, int t, int b, int r, int color,
                                     int bx, int by)
    {
        Left = l; Top = t; Bottom = b; Right = r;
        RectangleColor = color;
        BmpImgSx = bx;
        BmpImgSy = by;
        IsReadyForDraw = false;
    }

    public void SetReadyForDraw(boolean enable)
    {
        IsReadyForDraw = enable;
    }

    public  void Draw(Canvas cv, int CanvasSx, int CanvasSy)
    {
        if (IsReadyForDraw)
        {
            float fLeft, fTop,fRight, fBottom;
            Paint currentPaint;

            currentPaint = new Paint();
            currentPaint.setDither(true);
            currentPaint.setColor(RectangleColor);
            currentPaint.setStyle(Paint.Style.STROKE);
            currentPaint.setStrokeJoin(Paint.Join.ROUND);
            currentPaint.setStrokeCap(Paint.Cap.ROUND);
            currentPaint.setStrokeWidth(2);

            float ratioX = (float)BmpImgSx/CanvasSx;
            float ratioY = (float)BmpImgSy/CanvasSy;

            fLeft = Left *ratioX;
            fTop = Top *ratioY;
            fRight = Right *ratioX;
            fBottom = Bottom *ratioY;
            cv.drawRect(fLeft,fTop,fRight,fBottom,currentPaint);
        }
    }
}
