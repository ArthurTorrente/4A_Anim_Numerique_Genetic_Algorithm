# 4A_Anim_Numerique_Genetic_Algorithm

Deux projet sont présent dans le dossier de Rendu

1) Projet d'algorithme genetic pour le cours d'Animation numérique

Installation:
1) Ouvire le fichier install.bat et renseigner le chemin d'accès à la bibliothèque cinder, le chemin doit pointer sur le dossier racine de cinder (dossier include, boost, libr, src, ...)
2) ouvrir le projet visual 2013
3) Compiler le projet
4) Lancer l'application en Release avec l'utilisation de la carte graphique haute performance pour de meilleur performance

Une version compilé est disponible dans le dossier : Rendu/Genetic_Algorithm/bin

Utilisation:
Une fois l'application lancé il est possible de paramétré l'application à l'aide du panneau de gauche.
Ce panneau permet :
- Changement du mode d'input
    - Camera
    - Load d'image
- Afficher/Cacher les stats de l'application (FPS, FPS de l'algoGen)
- Mode load d'image
    - Loader une image en cliquant sur le bouton
    - Loader plusieurs image en drag and drop
        - L'appui sur la touche 'n' permet de switcher entre les diférentes images chargées
- Mode Camera
    - L'appui sur la touche 'c' permet de prendre une capture d'écran
    - Mode Temp réel

- Paramétrisaction de l'application
    - Choix du nombre de population qui sera utilisé par l'algoGen
    - Nombre de pixel qui seront utilisé par un Stixel (Moyenne des n pixel par un Stixel)
    - Espacement des Stixel
- Paramétrisation de l'algoGen
    - Pourcentage de Copy des meilleurs population
    - Pourcentage de Mutation des meilleur population
    - Pourcentage de Combinaison des meilleur population
    - Pourcentage de Génération de population aléatoire
    
- Controle de l'application
    - Lancement
    - Pause
        - Nexstep si l'application est en pause
    - Stop
    
Le pannaux en haut à droite permet le controle de la camera

Le pannaux Shader permet de paramétrer le shader:
- Nombre de voisins utilisé pour la moyenne des luminance
    
    
2) Projet de Valentin

Ne fonctionne que dans Visual, recompiler le projet en Release et l'executer directement dans la solution

Il est possiblde de translater la camera sur son axe local Z avec les touches S et W