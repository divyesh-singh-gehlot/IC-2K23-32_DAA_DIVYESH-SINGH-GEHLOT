// Binary Tree Traversals using Recursion
//   Preorder  : Root, Left, Right
//   Inorder   : Left, Root, Right
//   Postorder : Left, Right, Root

#include <iostream>
#include <queue>
using namespace std;

struct Node {
    int data;
    Node* left;
    Node* right;

    Node(int value) {
        data = value;
        left = NULL;
        right = NULL;
    }
};

Node* createTree(int arr[], int n) {
    if (n == 0 || arr[0] == -1) {
        return NULL;
    }
    Node* root = new Node(arr[0]);
    queue<Node*> q;
    q.push(root);
    int i = 1;
    while (!q.empty() && i < n) {
        Node* current = q.front();
        q.pop();
        if (i < n && arr[i] != -1) {
            current->left = new Node(arr[i]);
            q.push(current->left);
        }
        i++;
        if (i < n && arr[i] != -1) {
            current->right = new Node(arr[i]);
            q.push(current->right);
        }
        i++;
    }
    return root;
}

void preorder(Node* root) {
    if (root == NULL) {
        return;
    }
    cout << root->data << " ";     // visit the root first
    preorder(root->left);
    preorder(root->right);
}

void inorder(Node* root) {
    if (root == NULL) {
        return;
    }
    inorder(root->left);
    cout << root->data << " ";     // visit the root in the middle
    inorder(root->right);
}

void postorder(Node* root) {
    if (root == NULL) {
        return;
    }
    postorder(root->left);
    postorder(root->right);
    cout << root->data << " ";     // visit the root last
}

void deleteTree(Node* root) {
    if (root == NULL) {
        return;
    }
    deleteTree(root->left);
    deleteTree(root->right);
    delete root;
}

int main() {
    /*  The tree:
              1
             / \
            2   3
           / \   \
          4   5   6
    */
    int arr[] = {1, 2, 3, 4, 5, -1, 6};
    int n = sizeof(arr) / sizeof(arr[0]);

    Node* root = createTree(arr, n);

    cout << "Preorder  (Root Left Right): ";
    preorder(root);
    cout << endl;

    cout << "Inorder   (Left Root Right): ";
    inorder(root);
    cout << endl;

    cout << "Postorder (Left Right Root): ";
    postorder(root);
    cout << endl;

    deleteTree(root);
    return 0;
}
