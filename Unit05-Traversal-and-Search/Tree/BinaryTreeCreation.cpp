// Binary Tree Creation
// The tree is built from an array written level by level (left to right).
// -1 means "no node here". A queue remembers which node needs children next.

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

        // left child
        if (i < n && arr[i] != -1) {
            current->left = new Node(arr[i]);
            q.push(current->left);
        }
        i++;

        // right child
        if (i < n && arr[i] != -1) {
            current->right = new Node(arr[i]);
            q.push(current->right);
        }
        i++;
    }
    return root;
}

// prints the tree turned sideways (root on the left, right child on top)
void printTree(Node* root, int space) {
    if (root == NULL) {
        return;
    }
    printTree(root->right, space + 6);
    cout << string(space, ' ') << root->data << endl;
    printTree(root->left, space + 6);
}

int height(Node* root) {
    if (root == NULL) {
        return 0;
    }
    int l = height(root->left);
    int r = height(root->right);
    return 1 + (l > r ? l : r);
}

int countNodes(Node* root) {
    if (root == NULL) {
        return 0;
    }
    return 1 + countNodes(root->left) + countNodes(root->right);
}

int countLeaves(Node* root) {
    if (root == NULL) {
        return 0;
    }
    if (root->left == NULL && root->right == NULL) {
        return 1;
    }
    return countLeaves(root->left) + countLeaves(root->right);
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
    int arr[] = {1, 2, 3, 4, 5, -1, 6};
    int n = sizeof(arr) / sizeof(arr[0]);

    cout << "Level order input: ";
    for (int i = 0; i < n; i++) {
        cout << arr[i] << " ";
    }
    cout << endl << endl;

    Node* root = createTree(arr, n);

    cout << "Tree (sideways):" << endl;
    printTree(root, 0);

    cout << endl << "Number of nodes  : " << countNodes(root) << endl;
    cout << "Number of leaves : " << countLeaves(root) << endl;
    cout << "Height of tree   : " << height(root) << endl;

    deleteTree(root);
    return 0;
}
