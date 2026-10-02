#include <iostream>
using namespace std;

class Stack {
private:
    int* arr;
    int capacity;
    int topIndex;

public:
    Stack(int size) {
        capacity = size;
        arr = new int[capacity];
        topIndex = -1;
    }

    ~Stack() {
        delete[] arr;
    }

    bool isEmpty() const {
        return topIndex == -1;
    }

    bool isFull() const {
        return topIndex == capacity - 1;
    }

    void push(int value) {
        if (!isFull()) {
            arr[++topIndex] = value;
        }
    }

    int size() const {
        return topIndex + 1;
    }

    int get(int index) const {
        return arr[index];
    }

    void displayBottomToTop() const {
        for (int i = 0; i <= topIndex; i++) {
            cout << arr[i];
            if (i < topIndex) cout << " ";
        }
        cout << endl;
    }
};

int main() {
    int n;
    cout << "Enter number of elements: ";
    cin >> n;

    if (n <= 0) {
        cout << "Stack must contain at least one element." << endl;
        return 0;
    }

    Stack s(n);
    cout << "Enter " << n << " elements in bottom-to-top order:" << endl;

    for (int i = 0; i < n; i++) {
        int value;
        cin >> value;
        s.push(value);
    }

    int minimum = s.get(0);
    int maximum = s.get(0);

    for (int i = 1; i < s.size(); i++) {
        if (s.get(i) < minimum) minimum = s.get(i);
        if (s.get(i) > maximum) maximum = s.get(i);
    }

    cout << "\nStack (bottom -> top): ";
    s.displayBottomToTop();
    cout << "Maximum value: " << maximum << endl;
    cout << "Minimum value: " << minimum << endl;

    if (n % 2 == 1) {
        cout << "Middle element: " << s.get(n / 2) << endl;
    } else {
        cout << "Middle elements: " << s.get(n / 2 - 1)
             << " and " << s.get(n / 2) << endl;
    }

    return 0;
}
