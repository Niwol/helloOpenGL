# helloOpenGL

## Dépendances
La seule dépendance de ce projet est glfw3
```
sudo apt install libglfw3-dev
```

## Compilation
Le projet se compile avec CMake de manière très classique
```
cd path/to/project/root
mkdir build
cd build
cmake ..
make
```

## Controles utilisateurs
### Chargement de la scène
* 1-4 (sur le pavé numérique): Choix de la scène

### Contrôle de la caméra
* ZQSD : Mouvement de la caméra
* Espace/Ctrl-Gauche : Monté/déscente de la caméra
* Clic droit souris : rotation de la caméra

### Mode de rendu
* w : Changement de mode remplissage (plein ou vide)
* v : Rendu par défaut
* b : Rendu de la profondeur
* n : Rendu des normalse

### Autrs contrôles
* Click gauche de la souris : Tir de particule
* Echape ou click sur la croix pour quitter
