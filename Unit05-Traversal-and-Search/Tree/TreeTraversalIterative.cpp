// Binary Tree Traversals without recursion (using stacks)
// The recursive versions are included too, so the answers can be compared.

#include <iostream>
#include <queue>
#include <stack>
#include <vector>
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

// ---------- recursive (only used for comparison) ----------
void preorderRec(Node* root, vector<int>& result) {
    if (root == NULL) return;
    result.push_back(root->data);
    preorderRec(root->left, result);
    preorderRec(root->right, result);
}

void inorderRec(Node* root, vector<int>& result) {
    if (root == NULL) return;
    inorderRec(root->left, result);
    result.push_back(root->data);
    inorderRec(root->right, result);
}

void postorderRec(Node* root, vector<int>& result) {
    if (root == NULL) return;
    postorderRec(root->left, result);
    postorderRec(root->right, result);
    result.push_back(root->data);
}

// ---------- iterative ----------
vector<int> preorderIter(Node* root) {
    vector<int> result;
    if (root == NULL) {
        return result;
    }
    stack<Node*> s;
    s.push(root);
    while (!s.empty()) {
        Node* current = s.top();
        s.pop();
        result.push_back(current->data);
        // push right first so that left is taken out first
        if (current->right != NULL) s.push(current->right);
        if (current->left != NULL) s.push(current->left);
    }
    return result;
}

vector<int> inorderIter(Node* root) {
    vector<int> result;
    stack<Node*> s;
    Node* current = root;
    while (current != NULL || !s.empty()) {
        // go as far left as possible
        while (current != NULL) {
            s.push(current);
            current = current->left;
        }
        current = s.top();
        s.pop();
        result.push_back(current->data);
        current = current->right;
    }
    return result;
}

vector<int> postorderIter(Node* root) {
    vector<int> result;
    if (root == NULL) {
        return result;
    }
    // two stacks: s1 gives Root Right Left, and the second stack reverses it
    stack<Node*> s1, s2;
    s1.push(root);
    while (!s1.empty()) {
        Node* current = s1.top();
        s1.pop();
        s2.push(current);
        if (current->left != NULL) s1.push(current->left);
        if (current->right != NULL) s1.push(current->right);
    }
    while (!s2.empty()) {
        result.push_back(s2.top()->data);
        s2.pop();
    }
    return result;
}

void show(string name, vector<int> v) {
    cout << name;
    for (int i = 0; i < (int)v.size(); i++) {
        cout << v[i] << " ";
    }
    cout << endl;
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
    Node* root = createTree(arr, n);

    vector<int> pre = preorderIter(root);
    vector<int> in = inorderIter(root);
    vector<int> post = postorderIter(root);

    show("Preorder  (iterative): ", pre);
    show("Inorder   (iterative): ", in);
    show("Postorder (iterative): ", post);

    vector<int> preR, inR, postR;
    preorderRec(root, preR);
    inorderRec(root, inR);
    postorderRec(root, postR);

    cout << endl << "Same as recursive traversals: "
         << ((pre == preR && in == inR && post == postR) ? "Yes" : "No") << endl;

    deleteTree(root);
    return 0;
}
