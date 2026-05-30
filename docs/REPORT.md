# Report

## 1. Anàlisi de complexitat de la inicialització del mapa d'interseccions

El mapa d'interseccions es crea amb la funció build_intersection_graph(). Aquesta funció recorre tota la llista de carrers i, per a cada carrer, crida insert_hash_map() per afegir-lo a la taula hash.

Si anomenem n al nombre de carrers, la funció principal fa exactament una passada sobre la llista, per tant, hi ha n insercions.

### Millor cas

En el millor cas no hi ha col·lisions a la taula hash. Cada carrer es pot inserir directament a la seva posició corresponent i la cerca de la intersecció dins del bucket és constant.

Per tant:

**O(n)**

### Cas mitjà

La mida de la taula hash es crea com `1.5 * nombre_de_carrers`, de manera que la càrrega de la taula és baixa i les col·lisions solen ser poques. En aquestes condicions, cada inserció té cost constant de mitjana.

Per tant:

**O(n)**

### Pitjor cas

En el pitjor cas, totes les claus generades per la funció hash cauen al mateix bucket. Això provoca que cada inserció hagi de recórrer una llista cada vegada més gran per comprovar si la intersecció ja existeix.

La primera inserció costa O(1), la segona O(2)ñ..

El cost total seria:

**O(1 + 2 + 3 + ... + n)**

que equival a:

**O(n²)**

| Cas | Complexitat |
|------|-------------|
| Millor cas | O(n) |
| Cas mitjà | O(n) |
| Pitjor cas | O(n²) |

(On n és el nombre de carrers carregats)



## 2. Anàlisi de complexitat de la cerca de coordenades d'un carrer o lloc

Per trobar les coordenades d'una adreça o d'un lloc, el programa utilitza una cerca seqüencial sobre una llista enllaçada.

En el cas dels carrers, la funció find_house( recorre la llista de cases fins a trobar una coincidència entre el nom del carrer i el número de portal. De manera similar, la funció find_place() recorre la llista de llocs fins a trobar el nom indicat per l'usuari.

### Millor cas

Si l'element buscat es troba a la primera posició de la llista, la cerca finalitza immediatament.

Per tant:

**O(1)**

### Cas mitjà

De mitjana, caldrà recórrer aproximadament la meitat dels elements de la llista abans de trobar la coincidència.

Per tant:

**O(n)**

### Pitjor cas

Si l'element es troba a l'última posició o no existeix, serà necessari recórrer tota la llista.

Per tant:

**O(n)**

| Cas | Complexitat |
|------|-------------|
| Millor cas | O(1) |
| Cas mitjà | O(n) |
| Pitjor cas | O(n) |

On n és el nombre d'elements emmagatzemats a la llista (cases o llocs).

## 3.Anàlisi de complexitat de l'algorisme de cerca de camins

Per trobar el camí entre l'origen i el destí s'utilitza BFS sobre el graf d'interseccions.

El graf està emmagatzemat en una taula hash que permet obtenir els carrers connectats a una intersecció de manera eficient. La llista de nodes visitats, però, està implementada com una llista enllaçada.

### Millor cas

El millor cas es produeix quan l'origen i el destí coincideixen o es troben connectats directament. En aquest cas només es processen uns pocs nodes.

Per tant:

**O(1)**

### Cas mitjà

En una execució promedi, BFS visita una part significativa del graf. A més, cada comprovació de si un node ha estat visitat es fa recorrent una llista enllaçada.

- **V** = nombre d'interseccions (vèrtexs)
- **A** = nombre de carrers (arestes)

El cost dominant és la comprovació dels nodes visitats, que pot arribar a ser lineal respecte als nodes ja visitats.

Per tant, el cost mitjà és aproximadament:

**O(V² + A)**

### Pitjor cas

En el pitjor cas el destí és molt lluny o no existeix cap camí. BFS acaba recorrent pràcticament tot el graf.

Com que cada consulta a la llista de visitats té cost O(V), i aquesta operació es repeteix moltes vegades durant la cerca, el cost total arriba a:

**O(V² + A)**

| Cas | Complexitat |
|------|-------------|
| Millor cas | O(1) |
| Cas mitjà | O(V² + E) |
| Pitjor cas | O(V² + E) |

On V és el nombre d'interseccions del graf i E és el nombre de carrers del graf.

Si la llista de visitats es substituís per una taula hash, la complexitat del BFS passaria a ser:

**O(V + E)**

ja que les comprovacions de visitats serien constants de mitjana.

# 4. Estudi de latència: Cerca Seqüencial (Lab 4) vs. Hash Map (Lab 5)

## 4.1. Metodologia i Dades Rogues (Raw Data)

Per minimitzar interferències del sistema, s'ha mesurat el temps d'execució en microsegons utilitzant clock_gettime(CLOCK_MONOTONIC). Es fan 3 proves independents per a 4 mapes de mida progressiva, utilitzant l'opció 3 (Coordenades).

| Mapa | Carrers ($N$) | Execució | Latència Lab 4 (Seqüencial) [microsegons] | Latència Lab 5 (Hash Map) [microsegons] |
| :--- | :--- | :---: | :---: | :---: |
| **xs_1** | `13` | Exp 1 <br> Exp 2 <br> Exp 3 <br> **Mitjana** | `92` <br> `129` <br> `106` <br> **`109`** | `65` <br> `77` <br> `70` <br> **`70.66`** |
| **md_1** | `1318` | Exp 1 <br> Exp 2 <br> Exp 3 <br> **Mitjana** | `92` <br> `71` <br> `67` <br> **`76.66`** | `41` <br> `39` <br> `40` <br> **`40`** |
| **lg_1** | `3797` | Exp 1 <br> Exp 2 <br> Exp 3 <br> **Mitjana** | `78` <br> `109` <br> `114` <br> **`100.33`** | `41` <br> `45` <br> `41` <br> **`42.33`** |
| **xl_1** | `18944` | Exp 1 <br> Exp 2 <br> Exp 3 <br> **Mitjana** | `440` <br> `462` <br> `472` <br> **`458`** | `38` <br> `40` <br> `39` <br> **`39`** |

## 4.2. Anàlisi i Explicació dels Resultats


### 1. Comportament del Lab 4 (Cerca Seqüencial - O(N)$
Els resultats del Lab 4 mostren clarament com la latència depèn directament del volum de dades ($N$). 
* En els mapes xs_` (13 carrers), md_1 (1.318 carrers) i lg_1 (3.797 carrers), els temps de cerca es mantenen en valors molt baixos i similars (entre 76.66 i 109). A aquestes escales tan petites, el temps d'execució està altament influenciat pel "soroll" del sistema operatiu (gestió de memòria cau, interrupcions de la CPU) més que no pas pel recorregut de la llista.
* L'efecte de la complexitat lineal es fa totalment evident en fer el salt al mapa xl_1 (18.944 carrers). Aquí, el temps mitjà es dispara de manera contundent. El programa s'ha vist obligat a recórrer seqüencialment una llista enllaçada gairebé vint vegades més gran, fent salts de punter en memòria per a cada element, cosa que degrada el rendiment a mesura que el mapa creix.

### 2. Comportament del Lab 5 (Mapa d'Interseccions amb Taula Hash - O(1))

* Per al mapa més petit, el temps és de 70.66 (una mica superior al de mapes mitjans a causa de la càrrega inicial de l'estructura en memòria).
  Per als mapes md_1, lg_1 i xl_1, el temps es manté entre els 39 i 42 microsegons, tot i que el mapa xl_1 té gairebé 19.000 carrers més que el primer. 

Això es deu al fet que la funció de dispersió (hash function) calcula immediatament la posició de la intersecció buscada a la memòria (accés directe indexat). No importa com de gran sigui el mapa: el programa triga exactament el mateix a trobar les connexions perquè es dirigeix directament al bucket corresponent de la taula en lloc de buscar carrer per carrer.

### Conclusió
L'experiment mostra que per a mapes petits o mitjans la diferència és gairebé imperceptible. Tot i això, davant d'entorns realistes d'escala massiva, l'algorisme seqüencial del Lab 4 comença a perdre eficiència a ritme lineal, mentre que l'estructura de Taula Hash del Lab 5 garanteix una immediatesa absoluta (O(1)).

# 5. Millora de l'estructura de nodes visitats a l'algorisme BFS

### 5.1. Justificació
Actualment, l'algorisme de cerca utilitza una llista enllaçada simple (VisitedNode *visited) per emmagatzemar els identificadors de les interseccions ja explorades. Cada vegada que el BFS processa un node, crida a la funció is_visited(visited, id), la qual realitza un recorregut lineal de tota la llista per comprovar si l'ID ja existeix.

Per millorar la latència, s'hauria de substituir aquesta llista per una Taula Hash de nodes visitats o bé utilitzar un vector d'estats booleans indexat directament per ID, o una Taula Hash amb encadenament similar a la que ja s'ha implementat per al graf (IntersectionMap). Com que els ID de les interseccions d'OpenStreetMap són enters molt grans , una Taula Hash optimitzada amb col·lisió per encadenament o sondeig obert és l'estructura ideal per fer cerques rapides.

### 5.2. Complexitat algorísmica
* **Complexitat temporal actual:** La comprovació amb is_visite` sobre la llista té un cost de **O(V)**, on V és el nombre de nodes visitats en aquell moment del camí. Considerant que aquesta comprovació es realitza per a cada element extret de la cua i per a cada una de les seves connexions, la complexitat del BFS es degrada en el pitjor dels casos a O(V^2), on V és el nombre total de vèrtexs del graf.
* **Complexitat temporal millorada:** En utilitzar una Taula Hash de visitats, el cost de comprovar i afegir un element passa a ser de temps constant, O(1), en el cas mitjà. Això permet que l'algorisme BFS recuperi la seva complexitat òptima de O(V + E), on E és el nombre d'arestes (trams de carrer).

### 5.3. Tradeoffs i desavantatges
* **Latència:** La latència es redueix molt, especialment en mapes de gran escala com xl_1, on els camins BFS exploren milers de nodes abans de trobar el destí.
* **Memòria:** El principal desavantatge és l'increment en l'ús de memòria. Una llista enllaçada només allotja memòria per als nodes exactes que visita. Una taula Hash requereix reservar prèviament un espai de buckets buits (per exemple, proporcional a la mida del mapa) per minimitzar les col·lisions.

# 6. Millora de l'algorisme de localització del tram de carrer més proper

### 6.1. Justificació
Al codi, la funció find_closest_street(streets, user_pos) realitza una cerca lineal sobre tota la llista enllaçada de carrers carregats en memòria. Per a cada carrer, calcula el midpoint i la distància geomètrica haversine respecte a la posició de l'usuari per trobar el valor mínim.

Per optimitzar aquesta operació espacial, s'hauria d'implementar un Arbre K-dimensional en dues dimensions o un R-Tree. Els K-D Trees són estructures de dades de divisió de l'espai de forma binària que permeten organitzar punts geomètrics de manera jeràrquica. En lloc de calcular distàncies de forma massiva a cegues, l'arbre permet descartar regions senceres de l'espai que estan massa allunyades de la coordenada de l'usuari,

### 6.2. Complexitat algorísmica
* **Complexitat temporal actual:** La funció actual té un cost de O(N), on N és el nombre total de trams de carrer del mapa. Per a mapes com xl_1 amb 18.944 carrers, fer aquest bucle sencer requereix computar milers de vegades operacions trigonomètriques molt costoses.
* **Complexitat temporal millorada:** La cerca del veí més proper (Nearest Neighbor Search) en un k-d Tree equilibrat redueix el cost de temps a un nivell logarítmic O(log N) en el cas mitjà.

### 6.3. Tradefofs  i desavantatges
* **Latència:** La reducció de la latència és extrema en el moment d'iniciar la ruta. Passar de prop de 19.000 càlculs trigonomètrics del haversine a només uns 14 o 15 passos (on vas dividintl nombre de carrers entre 2 consecutivament) fa que la localització inicial passi de mil·lisegons a microsegons imperceptibles.
* **Memòria i Cost de Construcció:** El desavantatge d'aquesta solució és la complexitat d'implementació en C i el cost de temps requerit per inicialitzar i equilibrar l'arbre en el moment de carregar el mapa. L'estructura de l'arbre també afegeix un sobrecost en memòria degut als múltiples punters fills de cada node.
