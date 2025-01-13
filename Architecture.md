
# Architecture.md

## Introduction

Ce document explique la stratégie adoptée pour réaliser le projet, en détaillant l'architecture logicielle, les structures de données et les algorithmes principaux implémentés.

## 1. Architecture logicielle

La structure du projet peut être vue sous la forme d'une **pyramide**. On a un fichier `main.c`, incluant tous les autres fichiers et contenant la boucle principale permettant de faire tourner `fsh`. Chacun des autres fichiers gère une fonctionnalité isolée du projet.

Consulter le diagramme de classes à la fin de cette section pour une représentation visuelle de l'architecture.

- `main.c` :
    Point d'entrée principal du programme. Il initialise les composants essentiels, gère la boucle principale du shell, affiche et gère le prompt et transmet les commandes pour leur exécution.

- `commands.c` :
    Gère l'exécution des commandes internes, conditionnelles, en boucles ou externes.

- `tokenizer.c` :
    Réalise la tokenisation des commandes en arguments exploitables par les différents composants.

- `prompt.c`:
    Gère l'affichage dynamique du prompt du shell.

- `interns.c`  :
    Gère la logique derrière l'exécution des commandes internes du shell.
 
- `externs.c` :
    Gère la logique derrière l'exécution des commandes externes. L'exécution est gérée via des appels systèmes.
  
- `if.c` :
    Implémente la logique conditionnelle du shell, c'est-à-dire les `if` et les `else`.

- `for.c` :
    Implémente la gestion des boucles `for` dans le shell.

- `signaux.c` :
    Gère les signaux pour permettre une interruption propre des commandes en cours. 

- `redirections.c` :
    Implémente la gestion des redirections d'entrée/sortie standard et des erreurs.

- `pipeline.c` :
    Gère les pipelines permettant de chaîner plusieurs commandes via le caractère ' | '.


### Diagramme de classes
[Diagramme de classes](Architecture_de_classes.pdf)


## 2. Structures de données

#### 2.1 Tokens
- Description : Les commandes sont découpées en tokens (arguments) sous formes de tableaux de chaînes de caractères.

    - Exemple : `char *tokens[MAX_TOKENS]; int nb_tokens;`

#### 2.2. Command Buffer

- Description : Les commandes sont stockées dans des buffers temporaires pour permettre leur manipulation et leur modification (comme le remplacement de variables).

    - Exemple : `char command[MAX_LENGTH];` 

#### 2.3. Gestion des redirections

- Description : Utilisation de descripteurs de fichiers pour sauvegarder et restaurer les entrées/sorties standards.

    - Exemple : `static int saved_stdin, saved_stdout, saved_stderr;`

#### 2.4. Gestion des signaux

- Description : Des variables atomiques pour détecter et réagir aux signaux reçus.

    - Exemple : 
        - `volatile sig_atomic_t was_interrupted;`
        - `volatile sig_atomic_t last_signal;`


## 3. Algorithmes implémentés

#### 3.1. Tokenisation
- Description :
    Divise une commande en tokens en fonction d'un délimiteur spécifié (comme l'espace).

Algorithme :
    
    1. Utiliser `strtok()` pour découper la commande.

    2. Duplique chaque token dans un tableau dynamique.


#### 3.2. Gestion des commandes internes
- Description : 
    Exécute des commandes internes comme `cd`, `exit` ou `pwd`.

Algorithme :

    1. Identifier si la commande correspond à une commande interne.

    2. Appeler la fonction correspondante pour l'exécuter.


#### 3.3. Gestion des commandes externes
- Description : 
    Gère l'exécution des programmes externes via des appels système.

Algorithme :

    1. Localiser le programme via le `PATH`.

    2. Créer un processus enfant avec `fork()`.

    3. Exécuter la commande avec `execvp()`.

#### 3.4. Gestion du prompt

- Description : Gère dynamiquement l'affichage du prompt en fonction de l'état du shell.

Algorithme :

    1. Calculer la longueur dynamique du prompt (limité à 30 caractères).

    2. Troncature du répertoire courant si nécessaire.

    3. Ajouter un indicateur de statut (succès ou erreur), ou bien un affichage spécial en cas d’erreurs ou de signaux.


#### 3.5. Gestion des variables
- Description : Remplace les variables dans une commande par leur valeur correspondante.

Algorithme :

    1. Parcourir la commande pour identifier les occurrences de `$variable`.

    2. Utiliser un buffer temporaire pour reconstruire la commande :

        - Copier la partie avant `$variable`.

        - Insérer la valeur associée à `$variable`.

        - Copier la partie restante de la commande.

    3. Remplacer la commande originale par le contenu du buffer temporaire.


#### 3.6. Exécution conditionnelle
- Description : Gère les commandes conditionnelles if/else.

Algorithme :

    1. Identifier le début de la condition avec `if`.

    2. Analyser l'expression conditionnelle :

        - Exécuter la commande associée si la condition est vraie.

        - Si une clause `else` existe, exécuter la commande associée lorsque la condition est fausse.

    3. Gérer les imbrications potentielles des conditions.



#### 3.7. Gestion des boucles for
- Description : Implémente les boucles for dans le shell.

Algorithme :
    
    1. Analyser la syntaxe de la boucle pour identifier :

        - La variable de contrôle.

        - La liste des valeurs ou la plage à itérer.

    2. Pour chaque valeur :

        - Affecter la valeur à la variable de contrôle.

        - Exécuter le bloc de commandes associé.

    3. Gérer les options, les boucles imbriquées et les erreurs potentielles.


#### 3.8. Gestion des redirections
- Description : Gère les opérateurs comme <, >, >> pour rediriger les entrées/sorties.

Algorithme :

    1. Identifier les opérateurs dans les tokens.

    2. Ouvrir les fichiers appropriés avec `open()` et rediriger avec `dup2()`.

    3. Supprimer les tokens correspondant aux redirections.



#### 3.9. Gestion des signaux 
- Description : Configure des gestionnaires de signaux personnalisés et réagit aux interruptions.

Algorithme :

    1. Installer les gestionnaires avec `sigaction()`.

    2. Si l'exécution d'une commande est interrompue par la réception d'un signal `SIGTERM`, on l'indique à prompt en mettant `was_interrupted` à 1.

    3. Si on capte `SIGINT`, on arrête l'exécution, notamment les commandes structurées et on ne lance plus de nouveau processus.


#### 3.10. Gestion des pipelines
- Description : Implémente la communication entre commandes via les pipelines ' | '.

Algorithme :

    1. Créer des pipes pour chaque commande.

    2. Rediriger l'entrée/sortie standard des processus fils.

    3. Exécuter chaque commande et fermer les descripteurs inutilisés.

#### 3.11. Gestion des commandes multiples
- Description :
    Une commande composée de plusieurs sous-commandes séparées par des ' __;__ ' est décomposée et exécutée de manière séquentielle.

Algorithme :

    1. Diviser la commande à l'aide de `strtok()`.

    2. Appeler récursivement `execute_command()` pour chaque sous-commande.


#### 3.12. Gestion des commandes structurées
- Description : Gère les commandes imbriquées ou complexes comprenant plusieurs éléments comme des pipelines et des redirections.

Algorithme :

    1. Analyser la commande pour identifier sa structure.

    2. Appliquer les algorithmes appropriés à chaque sous-partie.

    3. Combiner les résultats pour produire l'exécution finale.

    4. La valeur de retour correspondra toujours à celle de la dernière exécution réussie. Cela permet de garder un résultat cohérent même en cas de réception d'un signal SIGINT.
