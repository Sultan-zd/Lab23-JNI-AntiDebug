package com.example.jnidemo;

import androidx.appcompat.app.AppCompatActivity;
import android.graphics.Color;
import android.os.Bundle;
import android.widget.TextView;
import java.util.Arrays;

public class MainActivity extends AppCompatActivity {

    @Override
    protected void onCreate(Bundle savedInstanceState) {
        super.onCreate(savedInstanceState);
        setContentView(R.layout.activity_main);

        // Récupération du gestionnaire natif (Singleton - Étape 15, Variante D)
        NativeSecurityManager nsm = NativeSecurityManager.getInstance();

        // Récupération des vues
        TextView tvStatus = findViewById(R.id.tvStatus);
        TextView tvHello = findViewById(R.id.tvHello);
        TextView tvFact = findViewById(R.id.tvFact);
        TextView tvReverse = findViewById(R.id.tvReverse);
        TextView tvMatrix = findViewById(R.id.tvMatrix);
        TextView tvForbidden = findViewById(R.id.tvForbidden);
        TextView tvBenchmark = findViewById(R.id.tvBenchmark);

        // Étape 15 - Variante B & C : Analyse du code d'état détaillé
        int securityStatus = nsm.getSecurityStatusNative();
        updateSecurityUI(tvStatus, securityStatus);

        // Exécution des fonctionnalités (Étape 13 - Ne pas bloquer brutalement)
        tvHello.setText(nsm.helloFromJNI());

        // Tests 1, 2, 3 : Factorielle
        String factRes = "Test 1 (10): " + nsm.factorial(10) + 
                         "\nTest 2 (-5): " + nsm.factorial(-5) + 
                         "\nTest 3 (20): " + nsm.factorial(20);
        tvFact.setText(factRes);

        // Test 4 : Inversion
        tvReverse.setText("Test 4 (\"\"): '" + nsm.reverseString("") + "'");

        // Extension A : Matrices
        float[] mA = {1, 2, 3, 4}, mB = {5, 6, 7, 8};
        float[] res = nsm.multiplyMatrices(mA, 2, 2, mB, 2, 2);
        tvMatrix.setText("Extension A (Matrix): " + Arrays.toString(res));

        // Extension B : Caractères interdits
        tvForbidden.setText("Extension B (Has '!'): " + nsm.hasForbiddenChars("Hello!", "!"));

        // Extension C : Benchmark
        long start = System.nanoTime();
        nsm.performHeavyTask(1000000);
        long duration = (System.nanoTime() - start) / 1000;
        tvBenchmark.setText("Extension C (Benchmark): " + duration + " µs");
    }

    /**
     * Gère l'affichage professionnel des alertes de sécurité (Variante C).
     */
    private void updateSecurityUI(TextView tv, int status) {
        switch (status) {
            case NativeSecurityManager.STATUS_OK:
                tv.setText("ENVIRONNEMENT : SÉCURISÉ (OK)");
                tv.setTextColor(Color.parseColor("#2E7D32")); // Vert
                break;
            case NativeSecurityManager.STATUS_TRACE_DETECTED:
                tv.setText("ALERTE : DÉBOGUEUR DÉTECTÉ (ptrace)");
                tv.setTextColor(Color.parseColor("#EF6C00")); // Orange
                break;
            case NativeSecurityManager.STATUS_SUSPICIOUS_MAPS:
                tv.setText("ALERTE : OUTILS SUSPECTS DÉTECTÉS (/proc/maps)");
                tv.setTextColor(Color.parseColor("#EF6C00"));
                break;
            case NativeSecurityManager.STATUS_MULTIPLE_SIGNALS:
                tv.setText("DANGER : MULTIPLES MENACES DÉTECTÉES");
                tv.setTextColor(Color.RED);
                break;
            default:
                tv.setText("ÉTAT : INCONNU");
        }
    }
}
