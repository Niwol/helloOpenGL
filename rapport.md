# Rapport projet IG3

## Lien GitHub vers le projet:
https://github.com/Niwol/helloOpenGL

## Objectifs

Mes objectifs pour ce projet ont été de refactorer et corriger certaines parties de mon projet de l'année dernière. Ensuite je voulais implémenter les shadow volumes et un concept du cours de géométrie qui au final a été la génération de particules avec et sans resorts.

# Refactorisation de l'ancien projet
Il y a certaines choses que je n'aimais pas trop dans mon dernier projet. J'ai donc commencé le projet par réécrire ces choses:

### Shaders
Les shaders sont maintenant regroupés et gérés par le `ShaderManager`. Je ne crée donc plus shader par `RenderObjct`

### Matériaux
J'ai complètement réécrit la gestion des matériaux, avant c'était une structure qui contenait tout les informations pour tous les matériaux possible
```
struc Material
{
    vec3 diffuse;
    vec3 specular;
    int shinniness;

    float metallic;
    float roughtness;
    ...
}
```
Maintenant j'ai une interface dont chaque différent matériel va hérité.

### Lumières
Les lumières ont également été mal gérées. J'ai donc aussi réécrit l'entièreté de la gestion des lumières.

J'ai d'abord, comme pour les matériaux, créé une interface dont chaque type de lumière va hérité. Ensuite, j'ai ramené la boucle qui itérait sur les lumières sur le côté CPU.

### Renderer
Enfin, j'ai refactoré la fonction de rendu du renderer. Maintenant les `RenderObjects` ne sont plus enregistrés dans le renderer pour qu'il les dessine mais je passe une scène entière au renderer lors du rendu et ce serra uniquement cette scène qui serra dessinée.


# Shadow volumes
Pour implémenter les shadow volumes je mes suis basé sur les chapitre 39 et 40 de ce tutoriel https://ogldev.org/index.html

En soit la théorie des shadow volumes ne m'a pas posé énormément de problèmes. C'était très clair et bien expliqué dans le tutoriel. Cependant pour l'implémentation, ce qui m'a posé problème est la détection de la silhouette des objets car pour l'obtenir, il me fallait une information sur les adjacences des triangles dans les maillages qui n'existait pas dans ma structure de maillage.

Pour contourner ce problème, j'ai décidé de ne pas faire un volume entier pour un objet mais un volume par triangle qui a une normale orienté vers la lumière. Cela n'a aucun impact sur l'algorithme des shadow volumes mais ça génère plus de volumes d'ombres pour un objet que nécessaire.

# Particules
Pour les particules je me suis principalement basé sur ce que nous avons vu en cours. J'ai donc uniquement créé une classe `Particle` qui contient un `RenderObject` et qui peut être mis à jour avec un `deltaTime`. J'ai implémenté les mises à jour avec du Euler explicite et Semi-implicite.

Je me suis pas penché sur le Euler implicite car d'un côté c'est quelque chose qui est rarement utilisé et d'un autre côté j'ai eu du mal à voir comment je pourrais obtenir la sommes des forces au temps t+1 pour une particule avec ma structure de code.

Ensuite j'ai fait une classe qui génère des particules pour faire des fontaines et j'ai fini par créer des ressorts encore avec les formules que nous avons vus en cours.

# Conclusion
Pour conclure je dirais que ce qui m'a pris le plus de temps était la refactorisation du projet de l'année dernière car il y avait pas mal choses à réécrir et à bien penser la structure du code (sans oublier tous les petit bugs qui arrivent créent et qui sont plus ou moins vite corigés).

Pour ce qui est des shadow volumes et des particules, j'ai eu beaucoup moins de bugs et la théorie derrière ces concepts était également très claire. Je n'ai donc pas passé autant de temps sur ces parties que sur la première partie.