# Projekt 1 -- Testování
# Úvod
Cílem tohoto projektu je si prakticky vyzkoušet typické úlohy spojené s testováním software pomocí tzv. unit testů. 
Projekt je rozdělen do tří částí, jejichž účelem je otestovat neznámý kód (black box testy), otestovat známý kód (white box testy), ověřit pokrytí kódu testy (code coverage) a implementovat kód na základě testů (test driven development). 
Za každou z těchto úloh je možné získat až **6 bodů**, za celý projekt lze tedy získat až **18 bodů**.

**Datum odevzdání:** 07.03.2023 23:59 <br>
Termín v IS bude těsně před deadlinem posunut na 08.03.2023 7:00.

**Archiv:** http://ivs.fit.vutbr.cz/projekt-testovani/ivs_project_1.zip

## Kontakty
* Martin Dočekal: idocekal@fit.vutbr.cz
* Karel Ondřej: ondrej@fit.vut.cz

## Použité nástroje
Jako implementační jazyk byl zvolen **C/C++**, (základní znalost by měla být dostatečná). 
Jako testovací framework je využívána kombinace:
* ```GoogleTest```
  [https://github.com/google/googletest/blob/master/docs/primer.md](https://github.com/google/googletest/blob/master/docs/primer.md) 
  [http://google.github.io/googletest/](http://google.github.io/googletest/)
* ```CMake/CTest```
  [https://cmake.org/documentation/](https://cmake.org/documentation/)
* ```GCOV```
  [https://gcc.gnu.org/onlinedocs/gcc/Gcov.html](https://gcc.gnu.org/onlinedocs/gcc/Gcov.html)

## Hodnocení
Pro hodnocení bude použit server **ivs.fit.vutbr.cz** a vaše řešení na tomto serveru tedy **závazně musí** fungovat. 
Tento server také můžete použít pro vyhodnocení pokrytí kódu (vizte dále).

# Struktura projektu

* Systém překladu (```CMake```): <br>
    Systém pro vygenerování skriptů pro překlad je tvořen soubory:
    ```CMakeLists.txt```, ```CMakeLists.txt.in```, ```CodeCoverage.cmake```, ```GoogleTest.cmake``` a nevyžaduje žádné úpravy.
* Úloha black-box testing: <br>
    Zdrojový soubor ```black_box_tests.cpp```, do kterého je třeba 
    doplnit příslušné testy a knihovna ```black_box_lib.a/.lib``` 
    s hlavičkovým souborem ```red_black_tree.h```, která obsahuje 
    testovaný kód binárního stromu.
* Úloha white-box testing: <br>
    Zdrojové soubory: ```white_box_code.h``` a ```white_box_code.cpp```, 
    které obsahují testovaný kód a ```white_box_tests.cpp```, do kterého 
    je třeba doplnit příslušné testy.
* Úloha test driven development: <br>
    Zdrojový soubor: ```tdd_code.h``` obsahuje definici rozhraní 
    prioritní fronty, kterou je třeba implementovat v ```tdd_code.cpp```. 
    Soubor ```tdd_test.cpp``` obsahuje testy prioritní fronty, které 
    musí vaše řešení splnit.
* **Odevzdávané soubory:** <br>
    Odevzdejte **POUZE** následující soubory zabalené do ```*.zip``` 
    archivu (lze provést přímo pomocí CMake -- vizte překlad a spuštění).
    * ```black_box_tests.cpp``` -- Testy Red-Black Tree
    * ```white_box_tests.cpp``` -- Testy maticových operací
    * ```tdd_code.h```, ```tdd_code.cpp``` -- Test Driven Development


# Prerekvizity a překlad
Projekt je určen pro platformu Linux/GCC.

## Prerekvizity
* GCC 5.4+ (mělo by být dostupné v repozitářích pro danou distribuci)
* GCOV (mělo by být dostupné v repozitářích pro danou distribuci)
* LCOV (v1.14 mělo by být dostupné v repozitářích pro danou distribuci)
* CMake 3.16+ (mělo by být dostupné v repozitářích pro danou distribuci)
* GoogleTest (CMake jej stáhne pomocí Git)
* Git client (opět by měl být v repozitářích)
* volitelně Doxygen pro generování dokumentace

## Překlad a spuštění

-# Soubor se zadáním projektu rozbalte do libovolného adresáře (například příkazem: ```unzip ivs_project_1.zip```, který archiv rozbalí v aktuálním adresáři).
-# Pokud nepoužíváte systémovou instalaci LCOV (například na serveru merlin) je nutné rozbalit staženou verzi do Vašeho adresáře např. pomocí: 

        $ tar -xvf lcov-1.14.tar.gz
        $ mv lcov-1.14 lcov
-# Přesuňte se do adresáře: ```build```, kde budou vytvořeny soubory překladu, výsledné spustitelné soubory a výstupy nástrojů pro analýzu pokrytí kódu (GCOV/LCOV).
-# Soubory pro překlad je možné vygenerovat příkazem: 

        $ cmake ..

    který vygeneruje Makefile (a stáhne GoogleTest) a měl by skončit výstupem: *Build files have been written to: ...*. 

    **Tento krok je nutné opakovat po každém přidání nového testu!**
-# Nyní by mělo být možné přeložit projekt příkazem: 

        $ make

    nebo 

        $ cmake --build .

    **Tento krok je nutné opakovat při každé změně zdrojového kódu!**
-# Testy je nyní možné zpustit příkazem:

        $ ctest -C Debug

    nebo každou část projektu odděleně přímo pomocí příslušných spustitelných souborů: ```./black_box_test``` pro *black-box testy*, ```./white_box_test``` pro *white-box testy* a ```./tdd_test``` pro úlohu *test driven development*.
-# Odevzdávaný archiv je možné vytvořit příkazem 

        $ cmake --build . --target pack

    (stále v adresáři ```build```), který vytvoří archiv ```xlogin00.zip```. 
    Archiv zkontrolujte a přejmenujte dle svého loginu.


### Ověření pokrytí kódu
Za předpokladu, že překlad a spuštění projektu proběhlo bez problémů, je nyní 
možné přistoupit k ověření pokrytí kódu testy pomocí nástrojů GCOV a LCOV. Pro 
tento účel vytváří CMake cíle ```white_box_test_coverage``` 
a ```tdd_test_coverage```. Analýzu pokrytí kódu je pak možné provést 
příkazem 

    $ make white_box_test_coverage

nebo

    $ make tdd_test_coverage

Výsledky analýzy by měly být v adresářích ```white_box_test_coverage```, 
nebo ```tdd_test_coverage``` ve formátu HTML a lze je zobrazit pomocí 
index.html v příslušném adresáři.

# Úlohy
V rámci projektu je třeba vypracovat tři úlohy zaměřené na testování: 
testování neznámého kódu (tzv. black box), testování známého kódu (tzv. white box) a implementace dle testů (tzv. test driven development).

## Black Box Testing (až 6 b.)
Cílem této úlohy je otestování základních operací nad neznámou implementací Red-Black tree (vizte [https://en.wikipedia.org/wiki/Red-black_tree](https://en.wikipedia.org/wiki/Red-black_tree)) na základě jejich definice. 

### Red-Black Tree
Red-Black Tree je stromová struktura (binární strom), která umožňuje provedení 
základních operací (vkládání, mazání a vyhledávání) v logaritmickém čase 
\f$\mathcal{O}(\log{}n)\f$. Struktura stromu je tvořena kolekcí uzlů, které jsou 
propojeny tak, že každý má maximálně dva potomky (levý a pravý). Každý uzel 
pak tvoří další podstrom (který může být prázdný). Každý strom má pak právě jeden 
uzel, který nemá žádného rodiče a nazývá se &bdquo;kořen/root&rdquo; (v obrázku 
níže má hodnotu 2).

\image html rb_tree.png Red-Black tree po vložení prvků 2,1 a 5. width=400
\image latex rb_tree.png Red-Black tree po vložení prvků 2,1 a 5.


### Implementujte testy
Vaším úkolem je implementovat testy základních operací nad Red-Black Tree (Insert, Find a Delete) a trojici základních axiomů které v této struktuře musí platit. 
Rozhraní stromu je definováno v souboru: ```red_black_tree.h``` (třída ```BinaryTree```). 
Vaše testy a inicializace testů doplňte do souboru: ```black_box_tests.cpp```, který je součástí odevzdávaného řešení.

\anchor rb_tree_axioms
### Základní axiomy Red-Black Tree
* Všechny listové uzly (tedy uzly bez potomků) jsou &bdquo;černé&rdquo;.
* Pokud je uzel &bdquo;červený&rdquo;, pak jsou jeho oba potomci &bdquo;černé&rdquo;.
* Každá cesta od každého listového uzlu ke kořeni obsahuje stejný počet &bdquo;černých&rdquo; uzlů (vizte oranžové cesty v obrázku).

### Hodnocení
Vaše testy musí být **závazně** pojmenovány tak, aby &bdquo;test case&rdquo; odpovídal 
názvu uvedenému u každé hodnocené položky a samotný název testu začínal označením 
testované vlastnosti uvedené tamtéž. Tedy například test vložení uzlu nad prázdným 
stromem: ```TEST(EmptyTree, InsertNode_XXX)```, kde ```XXX``` je libovolný 
další řetězec -- pro případ, že vlastnost testujete pomocí několika testů.

**Testy axiomů implementujte tak aby byly nezávislé na konkrétním obsahu stromu.**

* ```1,5 b```  Testy funkčnosti operací: ```InsertNode```, ```DeleteNode``` a ```FindNode``` nad prázdným stromem.
    * Název &bdquo;test case&rdquo;: ```EmptyTree```
    * Názvy testovaných vlastností: ```InsertNode```, ```DeleteNode```, ```FindNode```

* ```1,5 b```  Testy funkčnosti operací: ```InsertNode```, ```DeleteNode``` a ```FindNode``` nad NE-prázdným stromem.
    * Název &bdquo;test case&rdquo;: ```NonEmptyTree```
    * Názvy testovaných vlastností: ```InsertNode```, ```DeleteNode```, ```FindNode```

- ```3 b```  Testy 3 základních axiomů Red-Black Tree (vizte obrázek).
    - Název &bdquo;test case&rdquo;: ```TreeAxioms```
    - Názvy testovaných vlastností: ```Axiom1```, ```Axiom2```, ```Axiom3```


## White Box Testing (až 6b.)

Cílem této úlohy je otestování definovaných operací nad *hašovací tabulkou* na základě jejich známé implementace.

### Hašovací tabulka

Hašovací tabulka je vyhledávací datová struktura, která asociuje hašovací klíče s odpovídajícími hodnotami. 
Klíče v našem případě jsou řetězce (posloupnost charakterů proměnlivé délky) a jak již název napovídá, klíče jsou za účelem indexace převáděny na haše -- číselné hodnoty fixní délky. 
Na hašovací funkci nejsou kladeny stejné nároky jako v případě kryptografie, kdy dva podobné klíče musí mít výrazně odlišný haš, ale spíše na její rychlost.
Index hledané položky lze jednoduše získat jako zbytek po dělení haše a velikostí tabulky. 
V případě že dojde ke kolizi, tzn. dva různé klíče mají stejný haš nebo dva různé haše jsou mapovány na stejný index, je postupně otestován dle deterministického algoritmu další index až se nalezne hledaný záznam se stejným klíčem nebo volné místo v tabulce. 
Nejjednodušší možnost je postupně zkoušet indexy ```+1```, ```+2```, ```+3```, ...  dokud nenastane jedna ze zmíněných situací, nicméně tento přístup má několik nevýhod (Vizte https://hg.python.org/cpython/file/52f68c95e025/Objects/dictobject.c#l33).
V naší implementaci je využita pseudonáhodná řada popsaná následujícím algoritmem, kde ```hash``` odpovídá haši hledaného klíče a ```size``` velikosti tabulky:
```c
p := hash
i := hash mod size

while i neopovída indexu hledaného klíče nebo volnému místu
{
     i := (5 * i + 1 + p) mod size
     p := (p - (p mod 32)) / 32
}
```
Při vhodně zvolené velikosti indexu a implementaci hašovací funkce umožňuje tato datová struktura provádět základní operace jako vkládání, mazání a vyhledávání v konstantním čase \f$\mathcal{O}(1)\f$. 
Nicméně v praxi je velikost indexu omezený a u hašovací funkce dochází ke kolizím, takže pro nejhorší případ je horní odhad složitosti \f$\mathcal{O}(n)\f$ a průměrný \f$\Theta(1)\f$. 

\image html hashmap.png Ilustrace implementace hašovací tabulky pomocí seznamu a oboustraně vázaného seznamu. width=500
\image latex hashmap.png Ilustrace implementace hašovací tabulky pomocí seznamu a oboustraně vázaného seznamu.

### Implementujte testy

Vaším úkolem je implementovat testy základních operací nad hašovací tabulkou. 
Rozhraní operací je definováno v souboru: ```white_box_code.h```. 
Implementace operací je v souboru: ```white_box_code.cpp```. 
Vaše testy a inicializace testů doplňte do souboru: ```white_box_tests.cpp```, který je součástí odevzdaného řešení.


### Hodnocení
-# ```4 b.``` -- Rozsah pokrytí kódu pomocí testů.
-# ```2 b.``` -- Odhalení rozdílů implementace vůči specifikaci a otestování kolizí.

## Test Driven Development (až 6b)

V této úloze nebudete psát testy, ale přímo implementaci. 
Vaším úkolem je vytvořit třídu pro reprezentaci neorientovaného grafu, která bude splňovat dané rozhraní a testy.
Neorientovaný graf \f$G\f$ si definujme jako dvojici
$$ G = (V, E) \, . $$
Kde \f$V\f$ je konečná množina uzlů a \f$E \subseteq \left(\begin{matrix}V \\ 2 \\ \end{matrix}\right)\f$ je množin hran. 
\f$\left(\begin{matrix}V \\ 2 \\ \end{matrix}\right)\f$ je množina všech dvouprvkových podmnožin, tedy neuvažujeme smyčky (hrany z uzlu \f$u\f$ do \f$u\f$).

Takovýto graf lze reprezentovat maticí sousednosti nebo seznamem sousedů (Vizte ilustraci níže). 
Matice sousednosti obsahuje na daném místě jedničku, pokud existuje hrana mezi uzly \f$u\f$ a \f$v\f$. 
Jelikož my uvažujeme neorientovaný graf výsledná matice bude symetrická. 

Seznam sousedů je pak tradičně tvořen polem uzlů, které obsahují odkaz na jednosměrně vázaný seznam sousedů jednoho konkrétního uzlu. 
Není však nutné, aby seznam sousedů byl přímo implementován pomocí těchto struktur, ale je možné například jednosměrně vázaný seznam nahradit polem či jinou vhodnou datovou strukturou.

Oba způsoby mají své výhody a nevýhody. 
Matice se například více hodí pro husté grafy a seznam sousedů je naopak výhodnější pro grafy řídké. 
Hustý graf je takový graf, jehož počet hran \f$m\f$ se blíží \f$n^2\f$, kde \f$n\f$ je počet uzlů. 

\image html graph_repr.png Ilustrace reprezentace grafu pomocí seznamu sousedů a maticí sousednosti. width=500 
\image latex graph_repr.png Ilustrace reprezentace grafu pomocí seznamu sousedů a maticí sousednosti.


### Implementujte
Vaším úkolem je vytvořit implementaci neorientovaného grafu bez smyček v souborech ```tdd_code.h``` a ```tdd_code.cpp```. 
Tato implementace musí splňovat dodané testy v souboru ```tdd_tests.cpp``` a popis rozhranní uvedený přímo ve zdrojovém kódu.

### Popis struktur
**Reprezentace uzlu:** Uzel je reprezentován pomocí třídy ```Node```. Každý uzel obsahuje jednoznačný identifikátor ```id``` a barvu ```color```, která je reprezentována číselně. Identifikátor uzlu nemusí nutně odpovídat pořadí vložení uzlu do grafu, tedy například i uzel, který byl vložen jako první může mít ```id``` 42. Tedy jaké ```id``` má uzel záleží pouze na uživateli. 

**Reprezentace hrany:** Třída ```Edge```, která reprezentuje hranu v grafu, je již naimplementována a není nutné ji modifikovat.

**Reprezentace grafu:** Pro reprezentaci neorientovaného grafu bez smyček je zde třída ```Graph```. V dodaných souborech máte nadefinované rozhraní, které musí třída splňovat a vaším úkolem je doplnit implementaci. Jaký způsob reprezentace grafu zvolíte je na Vás.


### Hodnocení
* ```4,5 b.``` -- splnění testů (alespoň 12 ok jinak 0, testy pro Edge se do tohoto nepočítají)
* ```1,5 b.``` -- nedochází k problémům při práci s pamětí (memory leaks)

