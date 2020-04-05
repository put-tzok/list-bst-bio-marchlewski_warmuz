#include <assert.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <time.h>

unsigned int ns[] = {20000,23000,25000,27000,28500,30000,31000,32000};

// each tree node contains an integer key and pointers to left and right children nodes
struct node {
    int key;
    struct node *left;
    struct node *right;
};

// tree's beginning is called the root
struct node *root = NULL;


struct node **tree_search(struct node **candidate, int value) {
if(*candidate==NULL)//root nie istnieje
    return NULL;
else if((*candidate)->key>value){//poszukiwana wartość jest wieksza lub mniejsza od current key, rekurencynje sprawdza subdrzewa
    tree_search(&((*candidate)->left), value);}
else if((*candidate)->key<value){
    tree_search(&((*candidate)->right), value);
}else
return &(*candidate);//jesli value==key to zwraca poiter
}

struct node* nowy(int value){//dodanje nowy element drzewa

struct node *ptr =malloc(sizeof(struct node));
ptr->key=value;
ptr->left=NULL;
ptr->right=NULL;
if(root==NULL)//jeśli root nie isnieje to ustawia go
    root=ptr;
return ptr;//zwraca pointer na nowy element
}

void tree_node_sequel(struct node *ptr, int value){//umożliwa powiązanie nowych elemntów z ich rodzicem
struct node* temp= ptr;
if(value>temp->key){
        if(temp->right==NULL){
    temp->right=nowy(value);
}else
    tree_node_sequel(temp->right, value);}
else if(value<temp->key){
    if(temp->left==NULL){
        temp->left=nowy(value);
    }else
    tree_node_sequel(temp->left, value);

}
}




struct node* tree_insert(int value) {
    if(root==NULL){
       return nowy(value);
    }else{
    tree_node_sequel(root, value);
    }
    }



struct node **tree_maximum(struct node **candidate) {//znajduje bezposredni poprzednik
    if((*candidate)->right!=NULL)
        return tree_maximum(&(*candidate)->right);

    return &(*candidate);
}

void deletion(struct node* old, struct node* curr, int value){//usuwanie elementu
    int klucz;
if(curr==NULL)
    return;
if(curr->key==value){//key obecnie rozpatrywanego elemntu jest równy usuwanej wartości
   if(curr->left==NULL&&curr->right==NULL){//element do usunięcia nie ma dzieci
    if(root==curr)
    root=NULL;
    if(old->left==curr)//poprawne ustawienie pointera ewentualnego rodzica
    old->left=NULL;
    if(old->right==curr)
    old->right=NULL;
    free(curr);
    return;

   }else if(curr->left!=NULL&&curr->right==NULL){//usuwany element ma jedno dziecko
   if (curr==root){
    root=curr->left;
    free(curr);
    curr=NULL;
    return;}else{
        if(old->left==curr)
    old->left=curr->left;
    else
        old->right=curr->left;
    free(curr);
    curr=NULL;
    return;
    }

   }else if(curr->left==NULL&&curr->right!=NULL){
       if (curr==root){
    root=curr->right;
    free(curr);
    curr=NULL;
    return;}else{
        if(old->right==curr)
    old->right=curr->right;
    else
        old->left=curr->right;
    free(curr);
    curr=NULL;
    return;
    }

   }else{//usuwany element ma dwójke dzieci
   struct node** temp=tree_maximum(&(curr->left));//znalezienie poprzednika
    klucz=(*temp)->key;//przechowanie wartości

   deletion(root,root,klucz);//usuniecie poprzednika

   curr->key=klucz;//nadpisanie keya
   return;
   }
} else if (curr->key<value){//rekurencyjne poszukiwanie elementu do usunięcia
deletion(curr,curr->right,value);
} else if(curr->key>value){
deletion(curr,curr->left, value);
}


}



void tree_delete(int value){//funkcja startowa usuwania
deletion(root,root,value);
}

unsigned int tree_size(struct node *element){//rekurencujne zliczanie elemntów drzewa
if(element==NULL)
    return 0;
else
return 1+tree_size(element->left)+tree_size(element->right);
 }

/*
 * Fill an array with increasing values.
 *
 * Parameters:
 *      int *t:     pointer to the array
 *      int n:      number of elements in the array
 */
void fill_increasing(int *t, int n) {
    for (int i = 0; i < n; i++) {
        t[i] = i;
    }
}

/*
 * Reorder array elements in a random way.
 *
 * Parameters:
 *      int *t:     pointer to the array
 *      int n:      number of elements in the array
 */
void shuffle(int *t, int n) {
    for (int i = n - 1; i > 0; i--) {
        int j = rand() % i;
        int temp = t[i];
        t[i] = t[j];
        t[j] = temp;
    }
}

/*
 * Check if tree is a valid BST.
 *
 * Parameters:
 *      struct node *element:   pointer to node to be checked
 *
 * Returns:
 *      bool:                   true if subtree rooted in "element" is a BST
 */
bool is_bst(struct node *element) {
    // empty tree is a valid BST
    if (element == NULL) {
        return true;
    }
    // leaf node is valid
    if (element->left == NULL && element->right == NULL) {
        return true;
    }
    // only right subnode? check it recursively
    if (element->left == NULL && element->right != NULL) {
        return (element->key < element->right->key) && is_bst(element->right);
    }
    // only left subnode? check it recursively
    if (element->left != NULL && element->right == NULL) {
        return (element->key > element->left->key) && is_bst(element->left);
    }
    // both subnodes present? check both recursively
    return (element->key > element->left->key)
        && (element->key < element->right->key)
        && is_bst(element->left)
        && is_bst(element->right);
}

void insert_increasing(int *t, int n) {
    for (int i = 0; i < n; i++) {
        tree_insert(t[i]);
    }
}

void insert_random(int *t, int n) {
    shuffle(t, n);
    for (int i = 0; i < n; i++) {
        tree_insert(t[i]);
    }
}

void  binin(int *t, int s, int f){//zapełnianie zgodne z przepołowieniem
if(s==f)
    tree_insert(t[s]);
else{if((f-s)==1){
    tree_insert(t[s]);
    tree_insert(t[f]);
}else{
int q=s+((f-s)/2);
tree_insert(t[q]);
binin(t,s,q-1);
binin(t,q+1,f);
}
}
}


void insert_binary(int *t, int n) {//f. startowa

    binin(t, 0,n-1);
}

char *insert_names[] = { "Increasing", "Random", "Binary" };
void (*insert_functions[])(int*, int) = { insert_increasing, insert_random, insert_binary };

int main(int argc, char **argv) {
    for (unsigned int i = 0; i < sizeof(insert_functions) / sizeof(*insert_functions); i++) {
        void (*insert)(int*, int) = insert_functions[i];

        for (unsigned int j = 0; j < sizeof(ns) / sizeof(*ns); j++) {
            unsigned int n = ns[j];

            // crate an array of increasing integers: 0, 1, 2, ...
            int *t = malloc((n) * sizeof(int));

            fill_increasing(t, n);

            // insert data using one of methods
            clock_t insertion_time = clock();
            insert(t, n);
            insertion_time = clock() - insertion_time;

            assert(tree_size(root) == n);       // after all insertions, tree size must be `n`
            assert(is_bst(root));               // after all insertions, tree must be valid BST

            // reorder array elements before searching
            shuffle(t, n);

            // search for every element in the order present in array `t`
            clock_t search_time = clock();
            for (unsigned int k = 0; k < n; k++) {
                struct node **pnode = tree_search(&root, t[k]);
                struct node *iter = *pnode;
                assert(iter != NULL);           // found element cannot be NULL
                assert(iter->key == t[k]);      // found element must contain the expected value
            }
            search_time = clock() - search_time;

            // reorder array elements before deletion
            shuffle(t, n);

            // delete every element in the order present in array `t`
            for (unsigned int l = 0, m = n; l < n; l++, m--) {
                assert(tree_size(root) == m);   // tree size must be equal to the expected value
                tree_delete(t[l]);


                assert(is_bst(root));           // after deletion, tree must still be valid BST
            }

            assert(tree_size(root) == 0);
        // after all deletions, tree has size zero

            free(root);
            free(t);
            t=NULL;

            printf("%d\t%s\t%f\t%f\n",
                    n,
                    insert_names[i],
                    (double)insertion_time / CLOCKS_PER_SEC,
                    (double)search_time / CLOCKS_PER_SEC);
        }
    }
    return 0;
}
