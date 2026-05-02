package com.example.jnidemo;

/**
 * Manager centralisé pour les fonctions natives et la sécurité.
 * Conforme à l'Étape 15 (Variante D) du laboratoire.
 */
public class NativeSecurityManager {

    // Codes d'état (Étape 15 - Variante B)
    public static final int STATUS_OK = 0;
    public static final int STATUS_TRACE_DETECTED = 1;
    public static final int STATUS_SUSPICIOUS_MAPS = 2;
    public static final int STATUS_MULTIPLE_SIGNALS = 3;

    static {
        System.loadLibrary("native-lib");
    }

    /**
     * Retourne un code d'état détaillé sur la sécurité de l'environnement.
     */
    public native int getSecurityStatusNative();

    /**
     * Fonctions métier et tests du labo
     */
    public native String helloFromJNI();
    public native int factorial(int n);
    public native String reverseString(String s);
    public native int sumArray(int[] values);
    
    // Extensions
    public native float[] multiplyMatrices(float[] a, int r1, int c1, float[] b, int r2, int c2);
    public native boolean hasForbiddenChars(String str, String forbidden);
    public native void performHeavyTask(int iterations);

    // Singleton
    private static NativeSecurityManager instance;
    public static NativeSecurityManager getInstance() {
        if (instance == null) instance = new NativeSecurityManager();
        return instance;
    }
}
