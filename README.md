# helloOpenGL

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
### Chargement démo
* 0-9 (sur le pavé numérique): Choix de la démo

### Contrôle de la caméra
* Flèches directionnels : Mouvement de la caméra
* Clic souris et mollette : Mouvement de la caméra
* p : Changement du type de caméra

### Interaction dans la scène
* w : Changement de mode remplissage (plein ou vide)

#### Sélection d'objets
* r : objet sélectioné suivant
* y : objet sélectioné précédent
* t, f, g, h : sélection du point de controle sur l'objet sélectioné

#### Mouvements d'objets
* a : décendre l'objet sélectioné
* e : monter l'objet sélectioné
* z, q, s, d : mouvement de l'objet dans le plan xz

### Sélection des shaders
* x : shader basic, utilise la couleur diffuse pour déssiner les objets
* c : shader de lumière basique
* v : shader de texture
* b : shader des coordonnées uv
* n : shader des normales
* m : shader brdf

### Modification du material
* i : augmente le paramètre métallic de la surface
* k : baisse le paramètre métallic de la surface
* o : augmente le paramètre rugueux de la surface
* l : baisse le paramètre rugueux de la surface


## Structure du projet

J'ai repris le dépot github https://github.com/MathiasPaulin/helloOpenGL pour la base du projet. J'ai enlevé les 3 démos qui y étaient et j'ai ajouter une démo pour bézier. Ensuite j'ai ajouter le dossier lib qui contient tout mes implémentations de classes.

### Rendu
Tout ce qui concerne le rendu est géré par la classe `Renderer` à laquelle on peu ajouter et enlever des `RenderObjects` et des `PointLight`s. Dans le fonction `draw()`, le `Renderer` ajoute tout les lumières dans dans les shaders puis itère sur les `RenderObject`s pour les déssiner.

NB: La classe `Renderer` est une classe amie des classes `RenderObject`, `PointLight`, `Texture` et `Mesh`

* Un `RenderObject` a une références vers le `ShaderProgram` qui est utiliser pour le déssiner, un `Material`, un `Mesh` et sa matrice de modèle
* Un `PointLight` a une référence vers les `ShaderProgram`s qui utilisent cette lumière. Il a également des attributs de couleur, de position et un `RenderObject` qui peut être récupérer et ajouter au `Renderer`.


### Géométrie de bézier

Les classes `BezierCurve` et `BezierSurface` sont très similaires. Ils ont des fonctions d'ajout de point de contrôle, une fonction pour générer la courbe/surface...

Lors de la génération de courbe/surface, ils génèrent le `Mesh` des `RenderObject`s qu'ils ont en attribut. Ceux ci peuvent être récupérés et ajouter au `Renderer` pour l'affichage.


## Critique

### Héritage

Pour réaliser ce projet je me suis mis un challenge: je voulais évité un maximum d'utiliser de l'héritage. L'héritage est un concept que je n'apprécie pas spécialement et je voulais donc évité de l'utiliser (le fait de dire que je n'apprécie pas ce concept ne veut pas dire que je pense qu'il est inutile ou mauvais mais juste que personnellement je n'aime pas l'utiliser). Je pense avoir plustôt bien réussit à faire ce projet sans héritage. Cependant à la fin du projet, j'avais besoin d'une classe abstraite pour faire une interface. J'en avais besoin pour pouvoir sélectioner différents objets qui ont des éléments qui peuvent bouger (les points de contrôle des courbes et surface ainsi que la lumière).

### Améliorations possible

Il y a un certain nombre d'améliorations possible que j'aurais développer dans la suite du projet.

#### Gestion des données
En ce moment il y a beaucoup de copie de donnnée, surtout pour la classe `Mesh`. On pourra faire en sorte d'accéder directement aux buffers du mesh au lieu d'en créer un à l'exterieur et de le passer au copie au mesh qui le recopie encore une fois.

#### Lumières

L'implémentation de la lumière devra améliorer. Il faut améliorer la façon dont les lumères sont en relation avec les shaders, il faudrat que les shaders puissent facilement savoir combient de lumières sont utilisés (pour l'instant tout les shaders qui utilisent des lumières doivent utilisé exactement le même nombre de lumières).

On pourra aussi implémenté d'autres types de lumières, notament les directional lights et les spot lights

#### Construction d'objet

La contruction d'objet est assez fastidieuse. la plus part des objets (`BezierCurve`, `BezierSurface`, `RenderObject`...) doivent être contruit à la main. On pourra leurs donner des constructeurs par défaut ce qui simplifierais un peu ces tâches.

#### GUI

On aurais pu implémenté une interface avec des widgets pour l'utilisabilité de la démo au lieu de mapper tout les fonctionnalités sur les touches de clavier.

