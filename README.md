# JNI Demo Project

Ce projet est une démonstration de l'utilisation de **JNI (Java Native Interface)** dans une application Android. Il illustre comment appeler du code C++ depuis Java, gérer les types de données, et optimiser les performances via l'enregistrement dynamique des méthodes natives.

## Fonctionnalités de Base

1.  **Hello JNI** : Un simple "Hello World" provenant du monde natif.
2.  **Factorielle** :
    *   Gestion des valeurs normales (ex: 10! = 3628800).
    *   Gestion des erreurs pour les valeurs négatives (retourne -1).
    *   Détection de dépassement de capacité (overflow) pour les grands nombres (retourne -2).
3.  **Inversion de Chaîne** : Inverse une chaîne de caractères en C++, gère correctement les chaînes vides.
4.  **Somme de Tableau** : Calcule la somme des éléments d'un tableau d'entiers en natif.

## Extensions du Laboratoire

### Extension A : Multiplication Matricielle
Implémentation d'une multiplication de matrices (float) en C++. Démontre la manipulation de tableaux multidimensionnels via JNI.

### Extension B : Détection d'Erreurs
Vérifie si une chaîne contient des caractères interdits en utilisant les fonctions de recherche de la bibliothèque standard C++ (`std::string::find`).

### Extension C : Benchmark Java vs C++
Compare le temps d'exécution d'un calcul intensif (1 million d'itérations de calcul de racine carrée) entre Java et C++. Les résultats sont affichés en microsecondes.

### Extension D : Signature Dynamique (`RegisterNatives`)
Utilisation de `RegisterNatives` dans `JNI_OnLoad` pour lier les méthodes Java aux fonctions C++.
*   **Avantages** : Meilleure organisation du code, sécurité accrue (pas de noms de fonctions exportés par défaut), et légère amélioration des performances au premier appel.

## Structure du Projet

*   `app/src/main/java/com/example/jnidemo/MainActivity.java` : Interface utilisateur et appels JNI.
*   `app/src/main/cpp/native-lib.cpp` : Implémentations natives en C++.
*   `app/src/main/cpp/CMakeLists.txt` : Configuration de la compilation native.

## Comment compiler et exécuter
1. Ouvrez le projet dans Android Studio.
2. Assurez-vous que le NDK est installé.
3. Synchronisez Gradle.
4. Lancez l'application sur un émulateur ou un appareil physique.
