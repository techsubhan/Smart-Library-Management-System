#include <iostream>
#include <string>
#include <queue>
using namespace std;

// PART A: Singly Linked List (Book Records) 

struct Book {
    int id;
    string title;
    string author;
    string status;
    Book* next;
};

Book* head = nullptr;

void insertFront(int id, string title, string author) {
    Book* newBook = new Book();
    newBook->id = id;
    newBook->title = title;
    newBook->author = author;
    newBook->status = "Available";
    newBook->next = head;
    head = newBook;
    cout << "Book inserted at front.\n";
}

void insertLast(int id, string title, string author) {
    Book* newBook = new Book();
    newBook->id = id;
    newBook->title = title;
    newBook->author = author;
    newBook->status = "Available";
    newBook->next = nullptr;

    if (head == nullptr) {
        head = newBook;
        return;
    }
    Book* temp = head;
    while (temp->next != nullptr) {
        temp = temp->next;
    }
    temp->next = newBook;
    cout << "Book inserted at last.\n";
}

void insertSortedById(int id, string title, string author) {
    Book* newBook = new Book();
    newBook->id = id;
    newBook->title = title;
    newBook->author = author;
    newBook->status = "Available";

    if (head == nullptr || head->id >= id) {
        newBook->next = head;
        head = newBook;
        cout << "Book inserted in sorted order.\n";
        return;
    }
    Book* temp = head;
    while (temp->next != nullptr && temp->next->id < id) {
        temp = temp->next;
    }
    newBook->next = temp->next;
    temp->next = newBook;
    cout << "Book inserted in sorted order.\n";
}

void deleteFirst() {
    if (head == nullptr) {
        cout << "List is empty.\n";
        return;
    }
    Book* temp = head;
    head = head->next;
    delete temp;
    cout << "First book deleted.\n";
}

void deleteLast() {
    if (head == nullptr) {
        cout << "List is empty.\n";
        return;
    }
    if (head->next == nullptr) {
        delete head;
        head = nullptr;
        cout << "Last book deleted.\n";
        return;
    }
    Book* temp = head;
    while (temp->next->next != nullptr) {
        temp = temp->next;
    }
    delete temp->next;
    temp->next = nullptr;
    cout << "Last book deleted.\n";
}

void deleteById(int id) {
    if (head == nullptr) {
        cout << "List is empty.\n";
        return;
    }
    if (head->id == id) {
        Book* temp = head;
        head = head->next;
        delete temp;
        cout << "Book deleted.\n";
        return;
    }
    Book* temp = head;
    while (temp->next != nullptr && temp->next->id != id) {
        temp = temp->next;
    }
    if (temp->next == nullptr) {
        cout << "Book not found.\n";
        return;
    }
    Book* toDelete = temp->next;
    temp->next = toDelete->next;
    delete toDelete;
    cout << "Book deleted.\n";
}

void displayBooks() {
    if (head == nullptr) {
        cout << "No books in the library.\n";
        return;
    }
    cout << "\n----- Book Records -----\n";
    Book* temp = head;
    while (temp != nullptr) {
        cout << "ID: " << temp->id << " | Title: " << temp->title
             << " | Author: " << temp->author << " | Status: " << temp->status << "\n";
        temp = temp->next;
    }
}

void searchByTitle(string title) {
    Book* temp = head;
    while (temp != nullptr) {
        if (temp->title == title) {
            cout << "Found -> ID: " << temp->id << ", Author: " << temp->author
                 << ", Status: " << temp->status << "\n";
            return;
        }
        temp = temp->next;
    }
    cout << "Book not found.\n";
}

// updates status of a book in the linked list, used by Part D
void updateStatusInList(int id, string newStatus) {
    Book* temp = head;
    while (temp != nullptr) {
        if (temp->id == id) {
            temp->status = newStatus;
            return;
        }
        temp = temp->next;
    }
}

string getStatusFromList(int id) {
    Book* temp = head;
    while (temp != nullptr) {
        if (temp->id == id) return temp->status;
        temp = temp->next;
    }
    return "NOT_FOUND";
}


//  PART B: Binary Search Tree / AVL Tree (Search by ID) 

struct TreeNode {
    int id;
    string title;
    string author;
    string status;
    TreeNode* left;
    TreeNode* right;
    int height;
};

TreeNode* root = nullptr;

int treeHeight(TreeNode* node) {
    if (node == nullptr) return 0;
    return node->height;
}

int maxNum(int a, int b) {
    if (a > b) return a;
    return b;
}

TreeNode* rotateRight(TreeNode* y) {
    TreeNode* x = y->left;
    TreeNode* t2 = x->right;

    x->right = y;
    y->left = t2;

    y->height = maxNum(treeHeight(y->left), treeHeight(y->right)) + 1;
    x->height = maxNum(treeHeight(x->left), treeHeight(x->right)) + 1;

    return x;
}

TreeNode* rotateLeft(TreeNode* x) {
    TreeNode* y = x->right;
    TreeNode* t2 = y->left;

    y->left = x;
    x->right = t2;

    x->height = maxNum(treeHeight(x->left), treeHeight(x->right)) + 1;
    y->height = maxNum(treeHeight(y->left), treeHeight(y->right)) + 1;

    return y;
}

int getBalance(TreeNode* node) {
    if (node == nullptr) return 0;
    return treeHeight(node->left) - treeHeight(node->right);
}

TreeNode* insertNode(TreeNode* node, int id, string title, string author) {
    if (node == nullptr) {
        TreeNode* newNode = new TreeNode();
        newNode->id = id;
        newNode->title = title;
        newNode->author = author;
        newNode->status = "Available";
        newNode->left = nullptr;
        newNode->right = nullptr;
        newNode->height = 1;
        return newNode;
    }

    if (id < node->id) {
        node->left = insertNode(node->left, id, title, author);
    } else if (id > node->id) {
        node->right = insertNode(node->right, id, title, author);
    } else {
        return node;
    }

    node->height = 1 + maxNum(treeHeight(node->left), treeHeight(node->right));
    int balance = getBalance(node);

    // Left Left
    if (balance > 1 && id < node->left->id) return rotateRight(node);
    // Right Right
    if (balance < -1 && id > node->right->id) return rotateLeft(node);
    // Left Right
    if (balance > 1 && id > node->left->id) {
        node->left = rotateLeft(node->left);
        return rotateRight(node);
    }
    // Right Left
    if (balance < -1 && id < node->right->id) {
        node->right = rotateRight(node->right);
        return rotateLeft(node);
    }

    return node;
}

TreeNode* findMin(TreeNode* node) {
    while (node->left != nullptr) {
        node = node->left;
    }
    return node;
}

TreeNode* deleteNode(TreeNode* node, int id) {
    if (node == nullptr) return node;

    if (id < node->id) {
        node->left = deleteNode(node->left, id);
    } else if (id > node->id) {
        node->right = deleteNode(node->right, id);
    } else {
        if (node->left == nullptr || node->right == nullptr) {
            TreeNode* temp = node->left ? node->left : node->right;
            delete node;
            return temp;
        } else {
            TreeNode* temp = findMin(node->right);
            node->id = temp->id;
            node->title = temp->title;
            node->author = temp->author;
            node->status = temp->status;
            node->right = deleteNode(node->right, temp->id);
        }
    }

    if (node == nullptr) return node;

    node->height = 1 + maxNum(treeHeight(node->left), treeHeight(node->right));
    int balance = getBalance(node);

    if (balance > 1 && getBalance(node->left) >= 0) return rotateRight(node);
    if (balance > 1 && getBalance(node->left) < 0) {
        node->left = rotateLeft(node->left);
        return rotateRight(node);
    }
    if (balance < -1 && getBalance(node->right) <= 0) return rotateLeft(node);
    if (balance < -1 && getBalance(node->right) > 0) {
        node->right = rotateRight(node->right);
        return rotateLeft(node);
    }

    return node;
}

TreeNode* searchNode(TreeNode* node, int id) {
    if (node == nullptr || node->id == id) return node;
    if (id < node->id) return searchNode(node->left, id);
    return searchNode(node->right, id);
}

void updateStatusInTree(int id, string newStatus) {
    TreeNode* node = searchNode(root, id);
    if (node != nullptr) node->status = newStatus;
}

void inorderTraversal(TreeNode* node) {
    if (node == nullptr) return;
    inorderTraversal(node->left);
    cout << "ID: " << node->id << " | " << node->title << " | " << node->status << "\n";
    inorderTraversal(node->right);
}

void preorderTraversal(TreeNode* node) {
    if (node == nullptr) return;
    cout << "ID: " << node->id << " | " << node->title << " | " << node->status << "\n";
    preorderTraversal(node->left);
    preorderTraversal(node->right);
}

void postorderTraversal(TreeNode* node) {
    if (node == nullptr) return;
    postorderTraversal(node->left);
    postorderTraversal(node->right);
    cout << "ID: " << node->id << " | " << node->title << " | " << node->status << "\n";
}

void bfsTraversal(TreeNode* node) {
    if (node == nullptr) {
        cout << "Tree is empty.\n";
        return;
    }
    queue<TreeNode*> q;
    q.push(node);
    while (!q.empty()) {
        TreeNode* curr = q.front();
        q.pop();
        cout << "ID: " << curr->id << " | " << curr->title << " | " << curr->status << "\n";
        if (curr->left != nullptr) q.push(curr->left);
        if (curr->right != nullptr) q.push(curr->right);
    }
}


// ---------------- PART C: Queue (Reservation / Waiting List) ----------------

const int QUEUE_SIZE = 10;
int queueBookId[QUEUE_SIZE];
string queueMemberName[QUEUE_SIZE];
int frontIdx = -1;
int rearIdx = -1;

bool isQueueEmpty() {
    return frontIdx == -1;
}

bool isQueueFull() {
    return (rearIdx + 1) % QUEUE_SIZE == frontIdx;
}

void enqueue(int bookId, string memberName) {
    if (isQueueFull()) {
        cout << "Waiting list is full.\n";
        return;
    }
    if (isQueueEmpty()) {
        frontIdx = 0;
    }
    rearIdx = (rearIdx + 1) % QUEUE_SIZE;
    queueBookId[rearIdx] = bookId;
    queueMemberName[rearIdx] = memberName;
    cout << memberName << " added to waiting list for Book ID " << bookId << ".\n";
}

void dequeue() {
    if (isQueueEmpty()) {
        cout << "Queue is empty.\n";
        return;
    }
    cout << queueMemberName[frontIdx] << " removed from waiting list (Book ID "
         << queueBookId[frontIdx] << ").\n";
    if (frontIdx == rearIdx) {
        frontIdx = -1;
        rearIdx = -1;
    } else {
        frontIdx = (frontIdx + 1) % QUEUE_SIZE;
    }
}

void frontPeek() {
    if (isQueueEmpty()) {
        cout << "Queue is empty.\n";
        return;
    }
    cout << "Front: " << queueMemberName[frontIdx] << " (Book ID " << queueBookId[frontIdx] << ")\n";
}

void rearPeek() {
    if (isQueueEmpty()) {
        cout << "Queue is empty.\n";
        return;
    }
    cout << "Rear: " << queueMemberName[rearIdx] << " (Book ID " << queueBookId[rearIdx] << ")\n";
}

void displayQueue() {
    if (isQueueEmpty()) {
        cout << "No one is waiting.\n";
        return;
    }
    cout << "\n----- Waiting List -----\n";
    int i = frontIdx;
    while (true) {
        cout << queueMemberName[i] << " -> Book ID " << queueBookId[i] << "\n";
        if (i == rearIdx) break;
        i = (i + 1) % QUEUE_SIZE;
    }
}


//  PART D: Menus + Integration
void linkedListMenu() {
    int choice;
    do {
        cout << "\n--- Part A: Book Records (Linked List) ---\n";
        cout << "1. Insert at Front\n";
        cout << "2. Insert at Last\n";
        cout << "3. Insert Sorted by ID\n";
        cout << "4. Delete First\n";
        cout << "5. Delete Last\n";
        cout << "6. Delete by ID\n";
        cout << "7. Display All Books\n";
        cout << "8. Search by Title\n";
        cout << "0. Back\n";
        cout << "Choice: ";
        cin >> choice;

        int id;
        string title, author;

        if (choice == 1 || choice == 2 || choice == 3) {
            cout << "Enter Book ID: ";
            cin >> id;
            cout << "Enter Title: ";
            cin.ignore();
            getline(cin, title);
            cout << "Enter Author: ";
            getline(cin, author);
        }

        if (choice == 1) {
            insertFront(id, title, author);
            root = insertNode(root, id, title, author);
        } else if (choice == 2) {
            insertLast(id, title, author);
            root = insertNode(root, id, title, author);
        } else if (choice == 3) {
            insertSortedById(id, title, author);
            root = insertNode(root, id, title, author);
        } else if (choice == 4) {
            if (head != nullptr) root = deleteNode(root, head->id);
            deleteFirst();
        } else if (choice == 5) {
            if (head != nullptr) {
                Book* temp = head;
                while (temp->next != nullptr) temp = temp->next;
                root = deleteNode(root, temp->id);
            }
            deleteLast();
        } else if (choice == 6) {
            cout << "Enter Book ID to delete: ";
            cin >> id;
            root = deleteNode(root, id);
            deleteById(id);
        } else if (choice == 7) {
            displayBooks();
        } else if (choice == 8) {
            cout << "Enter title to search: ";
            cin.ignore();
            getline(cin, title);
            searchByTitle(title);
        }
    } while (choice != 0);
}

void treeMenu() {
    int choice;
    do {
        cout << "\n--- Part B: Book Search Tree (BST / AVL) ---\n";
        cout << "1. Search by ID\n";
        cout << "2. Inorder Traversal (DFS)\n";
        cout << "3. Preorder Traversal (DFS)\n";
        cout << "4. Postorder Traversal (DFS)\n";
        cout << "5. Level Order Traversal (BFS)\n";
        cout << "0. Back\n";
        cout << "Choice: ";
        cin >> choice;

        int id;
        if (choice == 1) {
            cout << "Enter Book ID: ";
            cin >> id;
            TreeNode* found = searchNode(root, id);
            if (found != nullptr) {
                cout << "Found -> Title: " << found->title << ", Status: " << found->status << "\n";
            } else {
                cout << "Book not found in tree.\n";
            }
        } else if (choice == 2) {
            cout << "\nInorder:\n";
            inorderTraversal(root);
        } else if (choice == 3) {
            cout << "\nPreorder:\n";
            preorderTraversal(root);
        } else if (choice == 4) {
            cout << "\nPostorder:\n";
            postorderTraversal(root);
        } else if (choice == 5) {
            cout << "\nBFS:\n";
            bfsTraversal(root);
        }
    } while (choice != 0);
}

void queueMenu() {
    int choice;
    do {
        cout << "\n--- Part C: Reservation Queue ---\n";
        cout << "1. Enqueue Member\n";
        cout << "2. Dequeue Member\n";
        cout << "3. Front Peek\n";
        cout << "4. Rear Peek\n";
        cout << "5. Is Empty?\n";
        cout << "6. Is Full?\n";
        cout << "7. Display Queue\n";
        cout << "0. Back\n";
        cout << "Choice: ";
        cin >> choice;

        int bookId;
        string name;

        if (choice == 1) {
            cout << "Enter Book ID: ";
            cin >> bookId;
            cout << "Enter Member Name: ";
            cin.ignore();
            getline(cin, name);
            enqueue(bookId, name);
        } else if (choice == 2) {
            dequeue();
        } else if (choice == 3) {
            frontPeek();
        } else if (choice == 4) {
            rearPeek();
        } else if (choice == 5) {
            if (isQueueEmpty()) cout << "Yes, queue is empty.\n";
            else cout << "No, queue is not empty.\n";
        } else if (choice == 6) {
            if (isQueueFull()) cout << "Yes, queue is full.\n";
            else cout << "No, queue is not full.\n";
        } else if (choice == 7) {
            displayQueue();
        }
    } while (choice != 0);
}

void issueBook() {
    int id;
    cout << "Enter Book ID to issue: ";
    cin >> id;

    string status = getStatusFromList(id);
    if (status == "NOT_FOUND") {
        cout << "Book not found.\n";
        return;
    }

    if (status == "Available") {
        updateStatusInList(id, "Issued");
        updateStatusInTree(id, "Issued");
        cout << "Book has been issued.\n";
    } else {
        cout << "Book is already issued. Adding member to waiting list.\n";
        string name;
        cout << "Enter Member Name: ";
        cin.ignore();
        getline(cin, name);
        enqueue(id, name);
    }
}

void returnBook() {
    int id;
    cout << "Enter Book ID to return: ";
    cin >> id;

    string status = getStatusFromList(id);
    if (status == "NOT_FOUND") {
        cout << "Book not found.\n";
        return;
    }

    updateStatusInList(id, "Available");
    updateStatusInTree(id, "Available");
    cout << "Book marked as returned.\n";

    if (!isQueueEmpty()) {
        // find the first waiting member for this exact book
        int i = frontIdx;
        bool matched = false;
        while (true) {
            if (queueBookId[i] == id) {
                matched = true;
                break;
            }
            if (i == rearIdx) break;
            i = (i + 1) % QUEUE_SIZE;
        }
        if (matched) {
            string memberName = queueMemberName[i];
            cout << memberName << " was waiting and now gets the book.\n";
            updateStatusInList(id, "Issued");
            updateStatusInTree(id, "Issued");
            dequeue();
        }
    }
}

int main() {
    int choice;
    do {
        cout << "\n===== Smart Library & Reading Management System =====\n";
        cout << "1. Book Records (Linked List)\n";
        cout << "2. Book Search Tree (BST / AVL)\n";
        cout << "3. Reservation Queue\n";
        cout << "4. Issue Book\n";
        cout << "5. Return Book\n";
        cout << "0. Exit\n";
        cout << "Choice: ";
        cin >> choice;

        if (choice == 1) linkedListMenu();
        else if (choice == 2) treeMenu();
        else if (choice == 3) queueMenu();
        else if (choice == 4) issueBook();
        else if (choice == 5) returnBook();
        else if (choice == 0) cout << "Exiting system. Goodbye!\n";
        else cout << "Invalid choice.\n";

    } while (choice != 0);

    return 0;
}
