package com.greenbit.MultiscanJNIGuiJavaAndroid;

import android.content.Context;
import android.content.SharedPreferences;
import android.os.Bundle;
import android.view.LayoutInflater;
import android.view.View;
import android.view.ViewGroup;
import android.widget.Toast;

import androidx.annotation.NonNull;
import androidx.annotation.Nullable;
import androidx.appcompat.widget.AppCompatButton;

import com.google.android.material.bottomsheet.BottomSheetDialogFragment;
import com.google.android.material.textfield.TextInputEditText;

import java.util.Objects;

public class BottomSheet extends BottomSheetDialogFragment {

    @Nullable
    @Override
    public View onCreateView(@NonNull LayoutInflater inflater, @Nullable ViewGroup container, @Nullable Bundle savedInstanceState) {
        //return super.onCreateView(inflater, container, savedInstanceState);
        View v = inflater.inflate(R.layout.bottom_sheet,
                container, false);

        AppCompatButton okay = v.findViewById(R.id.okay);
        TextInputEditText ip = v.findViewById(R.id.ip);


        okay.setOnClickListener(view -> {
            if (!Objects.requireNonNull(ip.getText()).toString().isEmpty()) {
                String toSharedPref = Objects.requireNonNull(ip.getText()).toString().trim();
                SharedPreferences sharedPref = getActivity().getPreferences(Context.MODE_PRIVATE);
                SharedPreferences.Editor editor = sharedPref.edit();
                //<string name="base_url">http://192.168.0.106:8000/api/</string>

                if (toSharedPref.contains(":")) {
                    Toast.makeText(getActivity(), "Please remove the PORT number", Toast.LENGTH_SHORT).show();
                } else {
                    editor.putString("address", "http://" + ip + ":8000/api/");
                    editor.apply();
                    Toast.makeText(getActivity(), "Done!", Toast.LENGTH_SHORT).show();
                    dismiss();
                }


            } else {
                Toast.makeText(getActivity(), "If left empty, the previous address will be used", Toast.LENGTH_SHORT).show();

            }

        });


        return v;
    }
}
