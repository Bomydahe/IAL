
/* ******************************* c206.c *********************************** */
/*  Předmět: Algoritmy (IAL) - FIT VUT v Brně                                 */
/*  Úkol: c206 - Dvousměrně vázaný lineární seznam                            */
/*  Návrh a referenční implementace: Bohuslav Křena, říjen 2001               */
/*  Vytvořil: Martin Tuček, říjen 2004                                        */
/*  Upravil: Kamil Jeřábek, září 2020                                         */
/*           Daniel Dolejška, září 2021                                       */
/* ************************************************************************** */
/*
** Implementujte abstraktní datový typ dvousměrně vázaný lineární seznam.
** Užitečným obsahem prvku seznamu je hodnota typu int. Seznam bude jako datová
** abstrakce reprezentován proměnnou typu DLList (DL znamená Doubly-Linked
** a slouží pro odlišení jmen konstant, typů a funkcí od jmen u jednosměrně
** vázaného lineárního seznamu). Definici konstant a typů naleznete
** v hlavičkovém souboru c206.h.
**
** Vaším úkolem je implementovat následující operace, které spolu s výše
** uvedenou datovou částí abstrakce tvoří abstraktní datový typ obousměrně
** vázaný lineární seznam:
**
**      DLL_Init ........... inicializace seznamu před prvním použitím,
**      DLL_Dispose ........ zrušení všech prvků seznamu,
**      DLL_InsertFirst .... vložení prvku na začátek seznamu,
**      DLL_InsertLast ..... vložení prvku na konec seznamu,
**      DLL_First .......... nastavení aktivity na první prvek,
**      DLL_Last ........... nastavení aktivity na poslední prvek,
**      DLL_GetFirst ....... vrací hodnotu prvního prvku,
**      DLL_GetLast ........ vrací hodnotu posledního prvku,
**      DLL_DeleteFirst .... zruší první prvek seznamu,
**      DLL_DeleteLast ..... zruší poslední prvek seznamu,
**      DLL_DeleteAfter .... ruší prvek za aktivním prvkem,
**      DLL_DeleteBefore ... ruší prvek před aktivním prvkem,
**      DLL_InsertAfter .... vloží nový prvek za aktivní prvek seznamu,
**      DLL_InsertBefore ... vloží nový prvek před aktivní prvek seznamu,
**      DLL_GetValue ....... vrací hodnotu aktivního prvku,
**      DLL_SetValue ....... přepíše obsah aktivního prvku novou hodnotou,
**      DLL_Previous ....... posune aktivitu na předchozí prvek seznamu,
**      DLL_Next ........... posune aktivitu na další prvek seznamu,
**      DLL_IsActive ....... zjišťuje aktivitu seznamu.
**
** Při implementaci jednotlivých funkcí nevolejte žádnou z funkcí
** implementovaných v rámci tohoto příkladu, není-li u funkce explicitně
 * uvedeno něco jiného.
**
** Nemusíte ošetřovat situaci, kdy místo legálního ukazatele na seznam
** předá někdo jako parametr hodnotu NULL.
**
** Svou implementaci vhodně komentujte!
**
** Terminologická poznámka: Jazyk C nepoužívá pojem procedura.
** Proto zde používáme pojem funkce i pro operace, které by byly
** v algoritmickém jazyce Pascalovského typu implemenovány jako procedury
** (v jazyce C procedurám odpovídají funkce vracející typ void).
**
**/

#include "c206.h"

int error_flag;
int solved;

/**
 * Vytiskne upozornění na to, že došlo k chybě.
 * Tato funkce bude volána z některých dále implementovaných operací.
 */
void DLL_Error() {
	printf("*ERROR* The program has performed an illegal operation.\n");
	error_flag = TRUE;
}

/**
 * Provede inicializaci seznamu list před jeho prvním použitím (tzn. žádná
 * z následujících funkcí nebude volána nad neinicializovaným seznamem).
 * Tato inicializace se nikdy nebude provádět nad již inicializovaným seznamem,
 * a proto tuto možnost neošetřujte.
 * Vždy předpokládejte, že neinicializované proměnné mají nedefinovanou hodnotu.
 *
 * @param list Ukazatel na strukturu dvousměrně vázaného seznamu
 */
void DLL_Init( DLList *list ) { //nastavenie pointerov v zozname na NULL

    list->activeElement = NULL; 
    list->lastElement = NULL;
    list->firstElement = NULL;
}

/**
 * Zruší všechny prvky seznamu list a uvede seznam do stavu, v jakém se nacházel
 * po inicializaci.
 * Rušené prvky seznamu budou korektně uvolněny voláním operace free.
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 */
void DLL_Dispose( DLList *list ) { //zrusi postupne vsetky prvku zoznamu

    while (list->firstElement != list->lastElement){
        DLLElementPtr elemPtr;
        if (list->firstElement != NULL){  
            elemPtr = list->firstElement; 
            if (list->activeElement == list->firstElement) 
                list->activeElement = NULL;                     
            else {
                list->firstElement = list->firstElement->nextElement; 
                list->firstElement->previousElement = NULL;}
            free(elemPtr);       
        }
    }
    free(list->firstElement); 
    list->activeElement = NULL; //nastavenie pointerov v zozname na NULL
    list->lastElement = NULL;
    list->firstElement = NULL;

}

/**
 * Vloží nový prvek na začátek seznamu list.
 * V případě, že není dostatek paměti pro nový prvek při operaci malloc,
 * volá funkci DLL_Error().
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 * @param data Hodnota k vložení na začátek seznamu
 */
void DLL_InsertFirst( DLList *list, int data ) {

    DLLElementPtr elementPTR = malloc(sizeof(struct DLLElement)); //alokacia pamati pre novy prvok
    if (elementPTR == NULL) // overenie uspesnosti alokacie
        DLL_Error();
    else { // vlozi prvok na zaciatok zoznamu v pripade, ze nieje prazdny
        elementPTR->data = data;
        elementPTR->nextElement = list->firstElement;
        elementPTR->previousElement = NULL;

        if (list->firstElement != NULL) { 
            list->firstElement->previousElement = elementPTR;          
            }
        else //ak je prazdny tak su pointre na prvy aj posledny prvok zhodne
            list->lastElement = elementPTR;
        
        list->firstElement = elementPTR; 
        }
}

/**
 * Vloží nový prvek na konec seznamu list (symetrická operace k DLL_InsertFirst).
 * V případě, že není dostatek paměti pro nový prvek při operaci malloc,
 * volá funkci DLL_Error().
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 * @param data Hodnota k vložení na konec seznamu
 */
void DLL_InsertLast( DLList *list, int data ) {

    DLLElementPtr elementPTR = malloc(sizeof(struct DLLElement));//alokacia pamati pre novy prvok
    if (elementPTR == NULL)// overenie uspesnosti alokacie
        DLL_Error();
    else {// vlozi prvok na koneic zoznamu v pripade, ze nieje prazdny
        elementPTR->data = data;
        elementPTR->nextElement = NULL;
        elementPTR->previousElement = list->lastElement;

        if (list->lastElement != NULL) { 
            list->lastElement->nextElement = elementPTR;          
            }
        else //ak je prazdny tak su pointre na prvy aj posledny prvok zhodne
            list->firstElement = elementPTR;
        
        list->lastElement = elementPTR; 
        }            
}

/**
 * Nastaví první prvek seznamu list jako aktivní.
 * Funkci implementujte jako jediný příkaz (nepočítáme-li return),
 * aniž byste testovali, zda je seznam list prázdný.
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 */
void DLL_First( DLList *list ) {

    list->activeElement = list->firstElement; //nastavi prvy prvok zoznamu na aktivny
}

/**
 * Nastaví poslední prvek seznamu list jako aktivní.
 * Funkci implementujte jako jediný příkaz (nepočítáme-li return),
 * aniž byste testovali, zda je seznam list prázdný.
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 */
void DLL_Last( DLList *list ) {

    list->activeElement = list->lastElement; //nastavi posledny prvok zoznamu na aktivny
}

/**
 * Prostřednictvím parametru dataPtr vrátí hodnotu prvního prvku seznamu list.
 * Pokud je seznam list prázdný, volá funkci DLL_Error().
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 * @param dataPtr Ukazatel na cílovou proměnnou
 */
void DLL_GetFirst( DLList *list, int *dataPtr ) {

    if (list->firstElement != NULL)  // overenie prazdnosti zoznamu
        *dataPtr = list->firstElement->data;   //ulozenie hodnoty prveho prvku zoznamu do dataPtr      
    else 
        DLL_Error();
}

/**
 * Prostřednictvím parametru dataPtr vrátí hodnotu posledního prvku seznamu list.
 * Pokud je seznam list prázdný, volá funkci DLL_Error().
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 * @param dataPtr Ukazatel na cílovou proměnnou
 */
void DLL_GetLast( DLList *list, int *dataPtr ) {

    if (list->firstElement != NULL)  // overenie prazdnosti zoznamu
        *dataPtr = list->lastElement->data;  //ulozenie hodnoty prveho prvku zoznamu do dataPtr         
    else 
        DLL_Error();
}

/**
 * Zruší první prvek seznamu list.
 * Pokud byl první prvek aktivní, aktivita se ztrácí.
 * Pokud byl seznam list prázdný, nic se neděje.
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 */
void DLL_DeleteFirst( DLList *list ) {
    
    DLLElementPtr elemPtr;
    if (list->firstElement != NULL){   // overi prazdnost zoznamu
        elemPtr = list->firstElement; 
        if (list->activeElement == list->firstElement) // ak je prvy aktivny, strati sa aj aktivita
            list->activeElement = NULL; 
        if (list->firstElement == list->lastElement){ // ak je tam len jeden element nastavuje sa pionter prveho aj posledneho prvku na NULL
            list->firstElement = NULL;
            list->lastElement = NULL;}
        else {
            list->firstElement = list->firstElement->nextElement; // zrusi prvy prvok zoznamu
            list->firstElement->previousElement = NULL;}
        free(elemPtr);       
    }    
}

/**
 * Zruší poslední prvek seznamu list.
 * Pokud byl poslední prvek aktivní, aktivita seznamu se ztrácí.
 * Pokud byl seznam list prázdný, nic se neděje.
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 */
void DLL_DeleteLast( DLList *list ) {

    DLLElementPtr elemPtr;
    if (list->lastElement != NULL){  // overi prazdnost zoznamu
        elemPtr = list->lastElement; 
        if (list->activeElement == list->lastElement) // ak je posledny aktivny, strati sa aj aktivita
            list->activeElement = NULL; 
        if (list->firstElement == list->lastElement){// ak je tam len jeden element nastavuje sa pionter prveho aj posledneho prvku na NULL
            list->firstElement = NULL;
            list->lastElement = NULL;}
        else {
            list->lastElement = list->lastElement->previousElement; // zrusi posledny prvok zoznamu
            list->lastElement->nextElement = NULL;}
        free(elemPtr);       
    }    
}

/**
 * Zruší prvek seznamu list za aktivním prvkem.
 * Pokud je seznam list neaktivní nebo pokud je aktivní prvek
 * posledním prvkem seznamu, nic se neděje.
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 */
void DLL_DeleteAfter( DLList *list ) {

    //odstrani prvok za aktivnym prvkom zoznamu
    if (list->activeElement != NULL) { // overenie ci ma zoznam aktivny prvok
        if (list->activeElement->nextElement != NULL) { // overenie ci ma prvok nejaky naslednovny prvok
            DLLElementPtr elemPtr;
            elemPtr = list->activeElement->nextElement; 
            list->activeElement->nextElement = elemPtr->nextElement;
            if (elemPtr == list->lastElement)
                list->lastElement = list->activeElement; 
            else
                elemPtr->nextElement->previousElement = list->activeElement;
            
            free(elemPtr); //uvolni prvok z pamate
        } 
    }
}

/**
 * Zruší prvek před aktivním prvkem seznamu list .
 * Pokud je seznam list neaktivní nebo pokud je aktivní prvek
 * prvním prvkem seznamu, nic se neděje.
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 */
void DLL_DeleteBefore( DLList *list ) {

    //odstrani prvok pred aktivnym prvkom zoznamu
    if (list->activeElement != NULL) { // overenie ci ma zoznam aktivny prvok
        if (list->activeElement->previousElement != NULL) { // overenie ci ma prvok nejaky predosly prvok
            DLLElementPtr elemPtr;
            elemPtr = list->activeElement->previousElement; 
            list->activeElement->previousElement = elemPtr->previousElement;
            if (elemPtr == list->firstElement)
                list->firstElement = list->activeElement; 
            else
                elemPtr->previousElement->nextElement = list->activeElement;
            
            free(elemPtr); //uvolni prvok z pamate
        } 
    }
    
}

/**
 * Vloží prvek za aktivní prvek seznamu list.
 * Pokud nebyl seznam list aktivní, nic se neděje.
 * V případě, že není dostatek paměti pro nový prvek při operaci malloc,
 * volá funkci DLL_Error().
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 * @param data Hodnota k vložení do seznamu za právě aktivní prvek
 */
void DLL_InsertAfter( DLList *list, int data ) {

    //vklada prvok za aktivny prvok zoznamu
    if (list->activeElement != NULL){ 
        DLLElementPtr elementPTR = malloc(sizeof(struct DLLElement)); //alokacia pamati pre novy prvok
        if (elementPTR == NULL) //overi uspesnost alokacie
            DLL_Error();
        else {
            elementPTR->data = data;
            elementPTR->nextElement = list->activeElement->nextElement;
            elementPTR->previousElement = list->activeElement;
            list->activeElement->nextElement = elementPTR;
            if (list->activeElement == list->lastElement)
                list->lastElement = elementPTR; 
            else
                elementPTR->nextElement->previousElement = elementPTR;
        }
        
    }
}

/**
 * Vloží prvek před aktivní prvek seznamu list.
 * Pokud nebyl seznam list aktivní, nic se neděje.
 * V případě, že není dostatek paměti pro nový prvek při operaci malloc,
 * volá funkci DLL_Error().
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 * @param data Hodnota k vložení do seznamu před právě aktivní prvek
 */
void DLL_InsertBefore( DLList *list, int data ) {

    //vklada prvok pred aktivny prvok zoznamu
    if (list->activeElement != NULL){ 
        DLLElementPtr elementPTR = malloc(sizeof(struct DLLElement));//alokacia pamati pre novy prvok
        if (elementPTR == NULL)//overi uspesnost alokacie
            DLL_Error();
        else {
            elementPTR->data = data;
            elementPTR->nextElement = list->activeElement;
            elementPTR->previousElement = list->activeElement->previousElement;
            list->activeElement->previousElement = elementPTR;
            if (list->activeElement == list->firstElement)
                list->firstElement = elementPTR; 
            else
                elementPTR->previousElement->nextElement = elementPTR;
        }
        
    }
    
}

/**
 * Prostřednictvím parametru dataPtr vrátí hodnotu aktivního prvku seznamu list.
 * Pokud seznam list není aktivní, volá funkci DLL_Error ().
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 * @param dataPtr Ukazatel na cílovou proměnnou
 */
void DLL_GetValue( DLList *list, int *dataPtr ) {

    if (list->activeElement == NULL) // overenie ci je zoznam aktivny
        DLL_Error ();
    else 
        *dataPtr = list->activeElement->data; // ulozi hodnotu do dataPtr a vrati ju
}

/**
 * Přepíše obsah aktivního prvku seznamu list.
 * Pokud seznam list není aktivní, nedělá nic.
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 * @param data Nová hodnota právě aktivního prvku
 */
void DLL_SetValue( DLList *list, int data ) {

    if (list->activeElement != NULL) // overenie ci je zoznam aktivny
        list->activeElement->data = data; // prepise hodnotu aktivneho prvku
}

/**
 * Posune aktivitu na následující prvek seznamu list.
 * Není-li seznam aktivní, nedělá nic.
 * Všimněte si, že při aktivitě na posledním prvku se seznam stane neaktivním.
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 */
void DLL_Next( DLList *list ) {

    if (list->activeElement != NULL){// overenie ci je zoznam aktivny
        if (list->activeElement == list->lastElement) // ak je aktivny posledny, aktivita sa stracia
            list->activeElement = NULL;
        else    
            list->activeElement = list->activeElement->nextElement; //presunie aktivitu na nasledujuci prvok
    }
}


/**
 * Posune aktivitu na předchozí prvek seznamu list.
 * Není-li seznam aktivní, nedělá nic.
 * Všimněte si, že při aktivitě na prvním prvku se seznam stane neaktivním.
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 */
void DLL_Previous( DLList *list ) {

    if (list->activeElement != NULL){// overenie ci je zoznam aktivny
        if (list->activeElement == list->firstElement) // ak je aktivny prvy, aktivita sa stracia
            list->activeElement = NULL;
        else    
            list->activeElement = list->activeElement->previousElement; //presunie aktivitu na predosli prvok
    }
}

/**
 * Je-li seznam list aktivní, vrací nenulovou hodnotu, jinak vrací 0.
 * Funkci je vhodné implementovat jedním příkazem return.
 *
 * @param list Ukazatel na inicializovanou strukturu dvousměrně vázaného seznamu
 *
 * @returns Nenulovou hodnotu v případě aktivity prvku seznamu, jinak nulu
 */
int DLL_IsActive( DLList *list ) {

    return list->activeElement != NULL; // overenie aktivnosti zoznamu
}

/* Konec c206.c */
