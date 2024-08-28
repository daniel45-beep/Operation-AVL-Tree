#include <stdio.h>
#include <stdlib.h>

// Struktur node untuk AVL Tree
typedef struct Node {
    int key;
    struct Node *left;
    struct Node *right;
    int height;
} Node;

// Fungsi untuk mendapatkan tinggi dari node
int height(Node *N) {
    if (N == NULL)
        return 0;
    return N->height;
}

// Fungsi untuk mendapatkan nilai maksimum dari dua bilangan
int max(int a, int b) {
    return (a > b) ? a : b;
}

// Fungsi untuk membuat node baru
Node* createNode(int key) {
    Node* node = (Node*)malloc(sizeof(Node));
    node->key = key;
    node->left = NULL;
    node->right = NULL;
    node->height = 1;  // Node baru awalnya adalah leaf node
    return(node);
}

// Fungsi untuk melakukan rotasi kanan
Node *rightRotate(Node *y) {
    Node *x = y->left;
    Node *T2 = x->right;

    // Lakukan rotasi
    x->right = y;
    y->left = T2;

    // Update tinggi
    y->height = max(height(y->left), height(y->right)) + 1;
    x->height = max(height(x->left), height(x->right)) + 1;

    // Return root baru
    return x;
}

// Fungsi untuk melakukan rotasi kiri
Node *leftRotate(Node *x) {
    Node *y = x->right;
    Node *T2 = y->left;

    // Lakukan rotasi
    y->left = x;
    x->right = T2;

    // Update tinggi
    x->height = max(height(x->left), height(x->right)) + 1;
    y->height = max(height(y->left), height(y->right)) + 1;

    // Return root baru
    return y;
}

// Mendapatkan balance factor dari node N
int getBalance(Node *N) {
    if (N == NULL)
        return 0;
    return height(N->left) - height(N->right);
}

// Fungsi untuk menyisipkan node baru ke dalam AVL Tree
Node* insert(Node* node, int key) {
    if (node == NULL)
        return createNode(key);

    if (key < node->key)
        node->left = insert(node->left, key);
    else if (key > node->key)
        node->right = insert(node->right, key);
    else
        return node;

    node->height = 1 + max(height(node->left), height(node->right));

    int balance = getBalance(node);

    // Kasus Left Left
    if (balance > 1 && key < node->left->key)
        return rightRotate(node);

    // Kasus Right Right
    if (balance < -1 && key > node->right->key)
        return leftRotate(node);

    // Kasus Left Right
    if (balance > 1 && key > node->left->key) {
        node->left = leftRotate(node->left);
        return rightRotate(node);
    }

    // Kasus Right Left
    if (balance < -1 && key < node->right->key) {
        node->right = rightRotate(node->right);
        return leftRotate(node);
    }

    return node;
}

// Fungsi untuk node minimum di subtree
Node *minValueNode(Node *node) {
    Node *current = node;
    while (current->left != NULL)
        current = current->left;
    return current;
}

// Fungsi untuk menghapus node dari AVL Tree
Node* deleteNode(Node* root, int key) {
    if (root == NULL)
        return root;

    if (key < root->key)
        root->left = deleteNode(root->left, key);
    else if (key > root->key)
        root->right = deleteNode(root->right, key);
    else {
        if ((root->left == NULL) || (root->right == NULL)) {
            Node *temp = root->left ? root->left : root->right;
            if (temp == NULL) {
                temp = root;
                root = NULL;
            } else
                *root = *temp;
            free(temp);
        } else {
            Node *temp = minValueNode(root->right);
            root->key = temp->key;
            root->right = deleteNode(root->right, temp->key);
        }
    }

    if (root == NULL)
        return root;

    root->height = 1 + max(height(root->left), height(root->right));

    int balance = getBalance(root);

    // Kasus Left Left
    if (balance > 1 && getBalance(root->left) >= 0)
        return rightRotate(root);

    // Kasus Left Right
    if (balance > 1 && getBalance(root->left) < 0) {
        root->left = leftRotate(root->left);
        return rightRotate(root);
    }

    // Kasus Right Right
    if (balance < -1 && getBalance(root->right) <= 0)
        return leftRotate(root);

    // Kasus Right Left
    if (balance < -1 && getBalance(root->right) > 0) {
        root->right = rightRotate(root->right);
        return leftRotate(root);
    }

    return root;
}

// Fungsi untuk mencari elemen dalam AVL Tree
Node* search(Node* root, int key) {
    if (root == NULL || root->key == key)
        return root;
    if (root->key < key)
        return search(root->right, key);
    return search(root->left, key);
}

// Fungsi untuk mencetak in-order traversal
void inOrder(Node *root) {
    if (root != NULL) {
        inOrder(root->left);
        printf("%d ", root->key);
        inOrder(root->right);
    }
}

// Fungsi untuk mencetak tinggi AVL Tree
void printHeight(Node* root) {
    if (root == NULL)
        printf("Tinggi pohon saat ini: 0\n");
    else
        printf("Tinggi pohon saat ini: %d\n", root->height);
}

// Fungsi menu utama
void menu() {
    Node *root = NULL;
    int choice, key;
    Node *found;

    while (1) {
        printf("\nMenu:\n");
        printf("1. Buat AVL Tree baru\n");
        printf("2. Sisipkan elemen baru\n");
        printf("3. Hapus elemen\n");
        printf("4. Cari elemen\n");
        printf("5. Cetak AVL Tree (in-order)\n");
        printf("6. Keluar\n");
        printf("Pilihan Anda: ");
        scanf("%d", &choice);

        switch (choice) {
            case 1:
                root = NULL;
                printf("AVL Tree baru telah dibuat.\n");
                break;
            case 2:
                printf("Masukkan elemen yang akan disisipkan: ");
                scanf("%d", &key);
                root = insert(root, key);
                printHeight(root);
                break;
            case 3:
                printf("Masukkan elemen yang akan dihapus: ");
                scanf("%d", &key);
                root = deleteNode(root, key);
                printHeight(root);
                break;
            case 4:
                printf("Masukkan elemen yang akan dicari: ");
                scanf("%d", &key);
                found = search(root, key);
                if (found != NULL)
                    printf("Elemen %d ditemukan di AVL Tree.\n", key);
                else
                    printf("Elemen %d tidak ditemukan di AVL Tree.\n", key);
                break;
            case 5:
                printf("In-order traversal dari AVL Tree:\n");
                inOrder(root);
                printf("\n");
                break;
            case 6:
                exit(0);
            default:
                printf("Pilihan tidak valid! Silakan coba lagi.\n");
        }
    }
}

int main() {
    menu();
    return 0;
}

