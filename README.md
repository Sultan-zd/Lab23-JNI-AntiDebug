# Lab23-JNI-AntiDebug

Ce projet est un laboratoire avancé sur l'utilisation de **JNI (Java Native Interface)** et le **Software Hardening** (durcissement logiciel) dans une application Android. Il démontre comment intégrer du code C++ natif pour des calculs intensifs et pour sécuriser l'application contre l'analyse dynamique (anti-debugging).

**GitHub Repository** : [https://github.com/Sultan-zd/Lab23-JNI-AntiDebug.git](https://github.com/Sultan-zd/Lab23-JNI-AntiDebug.git)

## Fonctionnalités Principales

### 1. Couche Native Professionnelle
*   **Gestionnaire Natif (Singleton)** : Utilisation d'une classe `NativeSecurityManager` en Java pour centraliser tous les appels JNI.
*   **Enregistrement Dynamique (`RegisterNatives`)** : Les méthodes natives sont liées dynamiquement au démarrage (`JNI_OnLoad`). Cela évite les noms de fonctions prévisibles (`Java_com_package...`) et améliore la sécurité contre l'ingénierie inverse.

### 2. Sécurité et Anti-Débogage
L'application analyse son environnement et renvoie un code d'état détaillé :
*   **Code 0 (OK)** : Environnement sain.
*   **Code 1 (Trace detected)** : Détection d'un débogueur via `ptrace`.
*   **Code 2 (Suspicious Maps)** : Détection d'outils d'instrumentation (Frida, Xposed, Magisk) dans `/proc/self/maps`.
*   **Code 3 (Multiple Threats)** : Plusieurs signaux d'alerte détectés.

### 3. Fonctionnalités Métier & Tests JNI
*   **Mathématiques** : Calcul de factorielle avec gestion d'erreur (négatif) et détection d'overflow (retourne -2 si > `INT_MAX`).
*   **Manipulation de Strings** : Inversion de chaîne sécurisée (gère les chaînes vides).
*   **Algèbre Linéaire** : Multiplication matricielle native (Float Array).
*   **Benchmark** : Comparaison de performance entre Java et C++ (calcul de 1,000,000 de racines carrées).

## Structure du Projet

*   `app/src/main/java/.../NativeSecurityManager.java` : Singleton gérant la liaison native.
*   `app/src/main/cpp/native-lib.cpp` : Implémentation C++ sécurisée avec `JNI_OnLoad`.
*   `app/src/main/res/layout/activity_main.xml` : Interface avec bandeau de statut dynamique (Couleurs : Vert/Orange/Rouge).

## Scénarios de Validation

### Scénario 1 : Lancement Normal
*   **Action** : Cliquer sur **Run** (Triangle vert).
*   **Résultat** : Statut "ENVIRONNEMENT : SÉCURISÉ (OK)" en vert.

### Scénario 2 : Lancement en Debug
*   **Action** : Cliquer sur **Debug** (Coccinelle).
*   **Résultat** : Statut "ALERTE : DÉBOGUEUR DÉTECTÉ" en orange/rouge.

### Scénario 3 : Analyse Logcat
Filtrez les logs avec le tag : `NativeSecurity`
```bash
adb logcat -s NativeSecurity
```
Vous verrez les détails de l'analyse : `Anti-Debug: ptrace check failed` ou `Environment check: OK`.

## Enseignements du Lab
Ce laboratoire illustre les étapes clés du développement sécurisé :
1. Séparer le code métier de la logique défensive.
2. Ne pas bloquer brutalement l'utilisateur mais adapter le comportement.
3. Utiliser les primitives système (`ptrace`, `maps`) pour un contrôle bas niveau.
4. Optimiser l'interface JNI via l'enregistrement dynamique.

---
*Développé dans le cadre du Lab23 JNI Anti-Debug.*
