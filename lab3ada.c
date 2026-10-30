
#include <stdio.h>
#include <stdlib.h>

typedef struct Node {
    int key;
    int height;
    struct Node *left;
    struct Node *right;
} Node;

int rotationCount = 0;

/* Return height of a node */
int getHeight(Node *node) {
    if (node == NULL)
        return 0;

    return node->height;
}

/* Maximum of two integers */
int max(int a, int b) {
    return (a > b) ? a : b;
}

/* Create a new node */
Node* createNode(int key) {
    Node *node = (Node*)malloc(sizeof(Node));

    node->key = key;
    node->height = 1;
    node->left = NULL;
    node->right = NULL;

    return node;
}

/* Right Rotation

        y                 x
       / \               / \
      x   T3    ->       T1  y
     / \                   / \
    T1 T2                 T2 T3
*/
Node* rightRotate(Node *y) {
    rotationCount++;

    Node *x = y->left;
    Node *T2 = x->right;

    x->right = y;
    y->left = T2;

    /* Update heights */
    y->height = 1 + max(getHeight(y->left),
                        getHeight(y->right));

    x->height = 1 + max(getHeight(x->left),
                        getHeight(x->right));

    return x;
}

/* Left Rotation

      x                       y
     / \                     / \
    T1  y       ->          x  T3
       / \                 / \
      T2 T3               T1 T2
*/
Node* leftRotate(Node *x) {
    rotationCount++;

    Node *y = x->right;
    Node *T2 = y->left;

    y->left = x;
    x->right = T2;

    /* Update heights */
    x->height = 1 + max(getHeight(x->left),
                        getHeight(x->right));

    y->height = 1 + max(getHeight(y->left),
                        getHeight(y->right));

    return y;
}

/* Get balance factor */
int getBalance(Node *node) {
    if (node == NULL)
        return 0;

    return getHeight(node->left) -
           getHeight(node->right);
}

/* AVL Insertion */
Node* insert(Node *node, int key) {

    /* Normal BST insertion */
    if (node == NULL)
        return createNode(key);

    if (key < node->key)
        node->left = insert(node->left, key);

    else if (key > node->key)
        node->right = insert(node->right, key);

    else
        return node;   // distinct keys

    /* Update height */
    node->height = 1 + max(getHeight(node->left),
                           getHeight(node->right));

    /* Balance factor */
    int balance = getBalance(node);

    /* LL Case */
    if (balance > 1 && key < node->left->key)
        return rightRotate(node);

    /* RR Case */
    if (balance < -1 && key > node->right->key)
        return leftRotate(node);

    /* LR Case */
    if (balance > 1 && key > node->left->key) {
        node->left = leftRotate(node->left);
        return rightRotate(node);
    }

    /* RL Case */
    if (balance < -1 && key < node->right->key) {
        node->right = rightRotate(node->right);
        return leftRotate(node);
    }

    return node;
}

/* Find minimum node */
Node* minValueNode(Node *node) {
    Node *current = node;

    while (current->left != NULL)
        current = current->left;

    return current;
}

/* AVL Deletion */
Node* deleteNode(Node *root, int key) {

    /* Normal BST deletion */
    if (root == NULL)
        return root;

    if (key < root->key)
        root->left = deleteNode(root->left, key);

    else if (key > root->key)
        root->right = deleteNode(root->right, key);

    else {

        /* Node with zero or one child */
        if (root->left == NULL ||
            root->right == NULL) {

            Node *temp;

            if (root->left != NULL)
                temp = root->left;
            else
                temp = root->right;

            if (temp == NULL) {
                /* No child */
                temp = root;
                root = NULL;
            }
            else {
                /* One child */
                *root = *temp;
            }

            free(temp);
        }

        else {
            /* Two children */

            Node *temp = minValueNode(root->right);

            root->key = temp->key;

            root->right =
                deleteNode(root->right, temp->key);
        }
    }

    /* If tree became empty */
    if (root == NULL)
        return root;

    /* Update height */
    root->height =
        1 + max(getHeight(root->left),
                getHeight(root->right));

    /* Get balance */
    int balance = getBalance(root);

    /* LL Case */
    if (balance > 1 &&
        getBalance(root->left) >= 0)
        return rightRotate(root);

    /* LR Case */
    if (balance > 1 &&
        getBalance(root->left) < 0) {

        root->left =
            leftRotate(root->left);

        return rightRotate(root);
    }

    /* RR Case */
    if (balance < -1 &&
        getBalance(root->right) <= 0)
        return leftRotate(root);

    /* RL Case */
    if (balance < -1 &&
        getBalance(root->right) > 0) {

        root->right =
            rightRotate(root->right);

        return leftRotate(root);
    }

    return root;
}

/* Inorder traversal */
void inorder(Node *root) {

    if (root == NULL)
        return;

    inorder(root->left);
    printf("%d ", root->key);
    inorder(root->right);
}

/* Preorder traversal */
void preorder(Node *root) {

    if (root == NULL)
        return;

    printf("%d ", root->key);
    preorder(root->left);
    preorder(root->right);
}

int main() {

    int values[] = {
        50, 30, 70, 20, 40, 60, 80,
        10, 5, 15, 25, 27, 26, 65,
        62, 64, 90, 100, 95
    };

    int n = sizeof(values) / sizeof(values[0]);

    Node *root = NULL;

    /* ---------------- INSERTION ---------------- */

    rotationCount = 0;

    for (int i = 0; i < n; i++) {
        root = insert(root, values[i]);
    }

    printf("After insertion:\n");

    printf("Inorder: ");
    inorder(root);

    printf("\nPreorder: ");
    preorder(root);

    printf("\nRoot = %d", root->key);

    printf("\nTotal insertion rotations = %d\n",
           rotationCount);


    /* ---------------- DELETION ---------------- */

    int deleteKeys[] = {
        70, 20, 50, 10, 90
    };

    int d = sizeof(deleteKeys) /
            sizeof(deleteKeys[0]);

    rotationCount = 0;

    for (int i = 0; i < d; i++) {

        root = deleteNode(root, deleteKeys[i]);

        printf("\nAfter deleting %d:", deleteKeys[i]);

        printf("\nInorder: ");
        inorder(root);

        printf("\nPreorder: ");
        preorder(root);
        printf("\n");
    }

    printf("\nTotal deletion rotations = %d\n",
           rotationCount);

    printf("Final root = %d\n", root->key);

    return 0;
}