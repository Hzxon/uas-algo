#include <stdio.h> 
#include <stdlib.h> 
#include <string.h> 

typedef struct movie {
    char judul[100];
    char genre[50];
    int durasi; 
    float rating;
    int upcoming; 
} movie;

typedef struct tbst {
    char judul[100];
    struct tbst *left, *right;  
} tbst; 

int read_data(movie *arr)
{
    FILE *fin = fopen("data_film.txt", "r");

    if (fin == NULL) {
        printf("Error opening resources data file!\n");
        return 0; 
    }

    int i = 0; 

    while(fscanf(fin, "%[^#]#%[^#]#%d#%f#%d\n", arr[i].judul, arr[i].genre, &arr[i].durasi, &arr[i].rating, &arr[i].upcoming) == 5)
        i++;

    fclose(fin);

    return i; 
}

void print_master_data(movie *arr, int n_data)
{
    if (n_data == 0 || arr == NULL) return; 

    for(int i = 0; i < n_data; i++){
        printf("%d. Judul : %s\n", i+1, arr[i].judul);
    } 
}

tbst *bst_createNewNode(movie data)
{
    tbst *newNode = (tbst *)malloc(sizeof(tbst));

    if (newNode == NULL){
        printf("Gagal mengalokasi memori!\n");
    }

    newNode->left = newNode->right = NULL; 
    strcpy(newNode->judul, data.judul);

    return newNode;
}

tbst *bst_insert(tbst *node, movie data)
{
    if (node == NULL) 
        return bst_createNewNode(data);

    int cmp = strcmp(data.judul, node->judul);

    if (cmp < 0) 
        node->left = bst_insert(node->left, data);
        
    else if (cmp > 0)
        node->right = bst_insert(node->right, data);

    return node; 
}

void inorder(tbst *node)
{
    if (node != NULL) {
        inorder(node->left);
        printf("Judul : %s\n", node->judul);
        inorder(node->right);
    }
}

void bst_searching(tbst *node, char searchKey[])
{
    if (node == NULL) return; 

    int cmp = strcmp(searchKey, node->judul);

    if (cmp == 0) {
        printf("Film berjudul %s ditemukan!\n", node->judul);
        return;
    } else if (cmp < 0) {
        bst_searching(node->left, searchKey);
    } else {
        bst_searching(node->right, searchKey);
    }
}

tbst *bst_minValueNode(tbst *node)
{
    while(node && node->left)
        node = node->left; 

    return node; 
}

tbst *bst_delete(tbst *node, char deleteKey[])
{
    if (node == NULL) return node;

    int cmp = strcmp(deleteKey, node->judul);

    if (cmp < 0) node->left = bst_delete(node->left, deleteKey);

    else if (cmp > 0) node->right = bst_delete(node->right, deleteKey);

    else {
        
        if (node->left == NULL) {
            tbst *temp = node->right; 
            free(node);
            return temp;  
        } 
        else if (node->right == NULL) {
            tbst *temp = node->left; 
            free(node);
            return temp;
        }
        
        // temp tidak mungkin NULL, karena sudah di cek di awal func. 
        tbst *temp = bst_minValueNode(node->right);

        strcpy(node->judul, temp->judul);

        node->right = bst_delete(node->right, temp->judul);
    }

    return node; 
}



int main(){

    movie *arr_master_data; 
    arr_master_data = malloc(sizeof(movie) * 100);
    int n_master_data = 0; 

    // fetching data
    n_master_data = read_data(arr_master_data);
    print_master_data(arr_master_data, n_master_data);


    // create bst 
    tbst *bst_root; 
    bst_root = NULL; 

    for(int i = 0; i < n_master_data; i++)
        bst_root = bst_insert(bst_root, arr_master_data[i]);

    printf("\n\n");
    inorder(bst_root);
} 
