package com.example.jnidemo;

import androidx.appcompat.app.AppCompatActivity;
import android.os.Bundle;
import android.widget.TextView;
import java.util.Arrays;

public class MainActivity extends AppCompatActivity {

    // Déclarations natives
    public native String helloFromJNI();
    public native int factorial(int n);
    public native String reverseString(String s);
    public native int sumArray(int[] values);

    // Extension A
    public native float[] multiplyMatrices(float[] a, int r1, int c1, float[] b, int r2, int c2);
    
    // Extension B
    public native boolean hasForbiddenChars(String str, String forbidden);
    
    // Extension C
    public native void performHeavyTask(int iterations);

    static {
        System.loadLibrary("native-lib");
    }

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        // Récupération des vues
        TextView tvHello = findViewById(R.id.tvHello);
        TextView tvFact = findViewById(R.id.tvFact);
        TextView tvReverse = findViewById(R.id.tvReverse);
        TextView tvArray = findViewById(R.id.tvArray);
        TextView tvMatrix = findViewById(R.id.tvMatrix);
        TextView tvForbidden = findViewById(R.id.tvForbidden);
        TextView tvBenchmark = findViewById(R.id.tvBenchmark);
        TextView tvRegister = findViewById(R.id.tvRegisterNatives);

        // 1-5 : Tests initiaux
        tvHello.setText(helloFromJNI());
        tvFact.setText("Fact(10)=" + factorial(10) + ", Fact(-5)=" + factorial(-5) + ", Fact(20)=" + factorial(20));
        tvReverse.setText("Reverse(\"\") = \"" + reverseString("") + "\"");
        tvArray.setText("Sum({}) = " + sumArray(new int[]{}));

        // Extension A : Matrix (2x2 * 2x2)
        float[] mA = {1, 2, 3, 4};
        float[] mB = {5, 6, 7, 8};
        float[] mRes = multiplyMatrices(mA, 2, 2, mB, 2, 2);
        tvMatrix.setText("Matrix Res: " + Arrays.toString(mRes));

        // Extension B : Forbidden chars
        boolean forbidden = hasForbiddenChars("Hello!", "!");
        tvForbidden.setText("Has '!' in 'Hello!': " + forbidden);

        // Extension C : Benchmark
        int iterations = 1_000_000;
        
        long startNative = System.nanoTime();
        performHeavyTask(iterations);
        long endNative = System.nanoTime();
        
        long startJava = System.nanoTime();
        performHeavyJavaTask(iterations);
        long endJava = System.nanoTime();
        
        tvBenchmark.setText("Benchmark (1M it.):\nNative: " + (endNative - startNative)/1000 + " µs\nJava: " + (endJava - startJava)/1000 + " µs");

        // Extension D : RegisterNatives status
        tvRegister.setText("Extension D: RegisterNatives active (check Logcat)");
    }

    private void performHeavyJavaTask(int iterations) {
        double result = 0.0;
        for (int i = 0; i < iterations; i++) {
            result += Math.sqrt(i);
        }
    }
}
