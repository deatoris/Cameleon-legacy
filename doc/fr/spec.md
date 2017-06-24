<h1>m2c</h1>
<strong>Méta Modèle</strong> d'un langage de programmation fonctionnelle, <strong>Caméléon</strong>.<br> 
m2c version 0.1.2 (alpha draft) du 14/11/2014 par O. Cugnon de Sévricourt<br><br>


<h2>Introduction</h2>
m2c permet de décrire une composition du langage Caméléon.<br>
Une composition fonctionnelle est une suite ordonnée et structurée de fonctions et d'écrans permettant l'exécution d'un processus métier particulier.<br>
<br>
m2c se base sur l'article "<a href="http://arxiv.org/abs/1110.4802">Cameleon language Part 1: Processor</a>" par O. Cugnon de Sévricourt et V. Tariel.<br>
<br>

<h2>Le modèle</h2>
Nous présentons deux modèles: un modèle se focalisant sur l'héritage entre concepts et un modèle graphique (diagramme de classe UML) se focalisation sur les compositions entre concepts.<br>
Nous détaillons ensuite les différents concepts.<br><br>
<img src="img/m2c-model-component.jpg" width="500" border="0"><br><br><br>
<center><i>Figure 1 - Modèle de dérivation</i></center><br><br>
<img src="img/m2c-model.jpg" width="500" border="n0"><br><br><br>
<center><i>Figure 2 - Modèle de composition/agrégation</i></center><br><br>

<h3>Le concept de composant, &lt;component&gt</h3>
Dans Caméléon, tout est composant.<br>
<br>
Un composant est:<br>
- un operateur &lt;operator&gt,<br>
- ou un controlleur &lt;control&gt,<br>
- ou une composition &lt;composition&gt,<br>
- ou une forme graphique &lt;shape&gt,<br>
- ou un texte d'illustration &lt;texte&gt.<br>
<br>
Une composition peut avoir été instancié à partir de l'un de ses dérivés instanciables, un schéma &lt;pattern&gt;.<br>
<br>

<strong>Remarque</strong><br> 
Il nous a semblé essentiel d'inclure certains éléments d'illustration (textes et formes) dans les composants constitutifs du langage.<br>
<br>

<h3>Le concept de composition, &lt;composition&gt</h3>
Une composition est un agréga de composants.<br>
Chaque composant a une position dans l'espace. Une implémentation spécifique a une représentation graphique particulière de chaque composant.<br>
<br>
<br>
Une composition peut inclure d'autres compositions. On parle de compositions inférieure pour les compositions incluses dans la composition courante. On parle de composition supérieure pour identifier la composition qui inclus la composition courante.<br>
<br>
<strong>Remarque</strong><br>
Chaque composant possède une représentation graphique ainsi qu'une position dans l'espace.<br>
Leur position et leur forme peuvent être déterminantes dans la compréhension de la composition.<br>
<br>

<h3>Le concept d'opérateur, &lt;operator&gt</h3>
Un opérateur est l'équivalent d'une fonction <br>
	* état<br>
	* condition d'execution<br>
	* fonction<br>
	* connecteurs (entrée & sortie)<br>
<br>

<h3>Le concept de connecteur, &lt;connector&gt</h3>
Un connecteur ...<br>
	* état<br>
	* donnée<br>
<br>

<h3>Le concept de connection, &lt;connexion&gt </h3>
<br>

<h3>Le concept de controlleur, &lt;control&gt</h3><br>
Un controlleur est l'équivalent d'un écran. Il peut avoir des connections en entrée et en sortie portant des structures de données de tous types. Une interaction utilisateur permet de modifier les données d'entrée afin de produire des données sortie.<br>
Cela peut également être une simple vue constituée uniquement de données de sorties ou un éditeur sans données d'entrée.<br>
	* fonction abstraite exécutée par l'utilisateur<br>
	* connecteurs (entrée & sortie)<br>
<br>
Pour un controlleur, la condition d'exécution équivaut à la condition d'exécution standard d'un opérateur.</strong><br>
<br>
Remarque: Il est possible de décrire une équivalence &lt;operator&gt/&lt;control&gt.</strong><br>
<br>

<h3>Le concept de schéma, &lt;pattern&gt</h3><br>
Un schéma implémente une composition instanciable à volonté (ie, pas de connection direct avec des composants inclus dans une composition supérieure).<br>
<br>

<h3>Le concept de forme graphique, &lt;shape&gt</h3>

<h3>Le concept de texte d'illustration, &lt;text&gt </h3>

<h2>L'implémentation spécifique Caméléon</h2>
La version actuelle (2.x.y) du langage Caméléon, implémentée par O. Cugnon de Sévricourt et V. Tariel, n'implémente pas rigoureusement m2c car m2c a été conçu en parallèle de la fabrication de Caméléon.<br><br>
Nous avons exposer précédemment le modèle conceptuel qu'il faut distinguer de l'implémentation spécifique de m2c réalisée dans le cadre de la fabrication de la version graphique de Caméléon.<br>
<br>
Nous faisons ici le rapprochement entre le modèle conceptuel décrit plus haut et la structure des fichiers représentant une composition et un pattern.<br>
Nous partirons d'exemples concrèts réalisé à l'aide de Caméléon. Nous exposons, pour chaque exemple, le rapprochement méta modèle/structure de fichier ainsi que la représentation graphique réalisée avec le langage.

<h3>Une composition élémentaire</h3>

<img src="img/Connection002.jpg" width="500" border="0"><br><br><br>


<h2>Références</h2>
* BPEL: http://fr.wikipedia.org/wiki/Business_Process_Execution_Language<br>
* YAWL: http://fr.wikipedia.org/wiki/YAWL<br>
* BPMN 1.X<br>
* UML 2.0<br>
<br>
<br>