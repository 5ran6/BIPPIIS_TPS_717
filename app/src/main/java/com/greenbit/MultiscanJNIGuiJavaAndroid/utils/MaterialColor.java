/*
 * Copyright 2019 Mountedwings Cybersystems LTD. All rights reserved.
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 * http://www.apache.org/licenses/LICENSE-2.0
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

package com.greenbit.MultiscanJNIGuiJavaAndroid.utils;

import android.content.Context;
import android.content.res.TypedArray;
import android.graphics.Color;

import java.util.Random;

public class MaterialColor {

    private static Random r = new Random();

    public static int randInt(int max) {
        return r.nextInt(max);
    }

    public static int getColor(Context ctx) {
        int returnColor = Color.WHITE;
        int arrayId = ctx.getResources().getIdentifier("mdcolor_random", "array", ctx.getPackageName());

        if (arrayId != 0) {
            TypedArray colors = ctx.getResources().obtainTypedArray(arrayId);
            int index = (int) (Math.random() * colors.length());
            returnColor = colors.getColor(index, Color.GRAY);
            colors.recycle();
        }
        return returnColor;
    }

    public static int getColor(Context ctx, String str, int index) {
        int returnColor = Color.WHITE;
        int arrayId = ctx.getResources().getIdentifier("mdcolor_random", "array", ctx.getPackageName());

        if (arrayId != 0) {
            TypedArray colors = ctx.getResources().obtainTypedArray(arrayId);
            int idx = index;
            while (idx >= colors.length()) {
                idx = idx - 5;
            }
            while (idx < 0) {
                idx = idx + 2;
            }
            returnColor = colors.getColor(idx, Color.GRAY);
            colors.recycle();
        }
        return returnColor;
    }

}
