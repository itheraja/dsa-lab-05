#include <iostream>
using namespace std;

struct Node {
    int data;
    Node* next;
};

class LinkedStack {
private:
    Node* topNode;
    int count;

public:
    LinkedStack() {
        topNode = NULL;
        count = 0;
    }

    ~LinkedStack() {
        while (topNode != NULL) {
            Node* temp = topNode;
            topNode = topNode->next;
            delete temp;
        }
    }

    void push(int value) {
        Node* newNode = new Node;
        newNode->data = value;
        newNode->next = topNode;
        topNode = newNode;
        count++;
    }

    int size() const {
        return count;
    }

    void copyBottomToTop(int output[]) const {
        Node* current = topNode;
        int index = count - 1;
        while (current != NULL) {
            output[index--] = current->data;
            current = current->next;
        }
    }

    void displayBottomToTop() const {
        if (count == 0) {
            cout << "Empty" << endl;
            return;
        }

        int* values = new int[count];
        copyBottomToTop(values);
        for (int i = 0; i < count; i++) {
            cout << values[i];
            if (i < count - 1) cout << " ";
        }
        cout << endl;
        delete[] values;
    }
};

class CircularQueue {
private:
    int* arr;
    int capacity;
    int front;
    int rear;
    int count;

public:
    CircularQueue(int size) {
        capacity = size;
        arr = new int[capacity];
        front = 0;
        rear = -1;
        count = 0;
    }

    ~CircularQueue() {
        delete[] arr;
    }

    bool isFull() const {
        return count == capacity;
    }

    bool isEmpty() const {
        return count == 0;
    }

    void enqueue(int value) {
        if (isFull()) return;
        rear = (rear + 1) % capacity;
        arr[rear] = value;
        count++;
    }

    void display() const {
        if (isEmpty()) {
            cout << "Empty" << endl;
            return;
        }

        for (int i = 0; i < count; i++) {
            cout << arr[(front + i) % capacity];
            if (i < count - 1) cout << " ";
        }
        cout << endl;
    }
};

void bubbleSort(int arr[], int n) {
    for (int i = 0; i < n - 1; i++) {
        for (int j = 0; j < n - i - 1; j++) {
            if (arr[j] > arr[j + 1]) {
                int temp = arr[j];
                arr[j] = arr[j + 1];
                arr[j + 1] = temp;
            }
        }
    }
}

int main() {
    int n1, n2;

    cout << "Enter size of Stack 1: ";
    cin >> n1;
    cout << "Enter size of Stack 2: ";
    cin >> n2;

    if (n1 <= 0 || n2 <= 0) {
        cout << "Both stacks must contain at least one element." << endl;
        return 0;
    }

    int* stack1 = new int[n1];
    int* stack2 = new int[n2];

    cout << "Enter Stack 1 elements in bottom-to-top order:" << endl;
    for (int i = 0; i < n1; i++) cin >> stack1[i];

    cout << "Enter Stack 2 elements in bottom-to-top order:" << endl;
    for (int i = 0; i < n2; i++) cin >> stack2[i];

    LinkedStack third;
    int i = 0, j = 0;

    while (i < n1 && j < n2) {
        third.push(stack1[i++]);
        third.push(stack2[j++]);
    }

    while (i < n1) third.push(stack1[i++]);
    while (j < n2) third.push(stack2[j++]);

    cout << "\nThird stack (bottom -> top): ";
    third.displayBottomToTop();

    int total = third.size();
    int* sorted = new int[total];
    third.copyBottomToTop(sorted);
    bubbleSort(sorted, total);

    cout << "Sorted elements (ascending): ";
    for (int k = 0; k < total; k++) {
        cout << sorted[k];
        if (k < total - 1) cout << " ";
    }
    cout << endl;

    CircularQueue queue(total);
    for (int k = 0; k < total; k++) {
        queue.enqueue(sorted[k]);
    }

    cout << "Circular queue (front -> rear): ";
    queue.display();

    delete[] stack1;
    delete[] stack2;
    delete[] sorted;
    return 0;
}
