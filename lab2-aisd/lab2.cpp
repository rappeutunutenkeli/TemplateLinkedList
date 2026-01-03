#include <iostream>
#include <cstdlib>
#include <sstream>
#include <string>
#include <ctime>

template<typename T>
class LinkedList {
private:

    template<typename T>
    struct Node {
        T data;
        Node* next;

        explicit Node(const T& value = T(), Node* nex = nullptr)
            : data(value), next(nex) {
        }
    };

    Node<T>* head;
    size_t count;


public:
    LinkedList() : head(nullptr), count(0) {}

    LinkedList(const LinkedList& other);

    ~LinkedList();

    LinkedList& operator=(const LinkedList&);

    void push_tail(const T& val);

    LinkedList<T> operator+(const LinkedList<T>& rhs);

    void push_head(const T& val);

    LinkedList<T>& operator+=(const LinkedList<T>& rhs);

    bool pop_head(T&);

    bool pop_tail(T&);

    void delete_node(const T&);

    const T& at(size_t index) const;
    T& at(size_t index);

    void generate_random_elements(int seed, int numberOfElements);

    size_t size() const { return count; }

   
    LinkedList<int> find_unlucky_numbers();
    static LinkedList<int> generate_sequence(int n);

  
    void remove_every_nth(int n);
};

template<typename T>
LinkedList<T>::LinkedList(const LinkedList<T>& other) : head(nullptr), count(other.count) {
    if (!other.head) return;

    head = new Node<T>(other.head->data);
    Node<T>* currentThis = head;
    Node<T>* currentOther = other.head->next;

    while (currentOther != nullptr) {
        currentThis->next = new Node<T>(currentOther->data);
        currentThis = currentThis->next;
        currentOther = currentOther->next;
    }
}

template<typename T>
LinkedList<T>::~LinkedList() {
    while (head != nullptr) {
        Node<T>* temp = head;
        head = head->next;
        delete temp;
    }
}

template<typename T>
LinkedList<T>& LinkedList<T>::operator=(const LinkedList<T>& rhs) {
    if (&rhs == this) return *this;

    while (head != nullptr) {
        Node<T>* temp = head;
        head = head->next;
        delete temp;
    }

    if (rhs.head != nullptr) {
        head = new Node<T>(rhs.head->data);
        Node<T>* currentThis = head;
        Node<T>* currentOther = rhs.head->next;

        while (currentOther != nullptr) {
            currentThis->next = new Node<T>(currentOther->data);
            currentThis = currentThis->next;
            currentOther = currentOther->next;
        }
    }

    count = rhs.count;
    return *this;
}

template<typename T>
void LinkedList<T>::push_tail(const T& val) {
    auto* node = new Node<T>(val);
    if (head == nullptr) {
        head = node;
    }
    else {
        Node<T>* last = head;
        while (last->next != nullptr) {
            last = last->next;
        }
        last->next = node;
    }
    ++count;
}

template<typename T>
LinkedList<T> LinkedList<T>::operator+(const LinkedList<T>& rhs) {
    LinkedList<T> result(*this);
    for (size_t i = 0; i < rhs.size(); ++i) {
        result.push_tail(rhs.at(i));
    }
    return result;
}

template<typename T>
void LinkedList<T>::push_head(const T& val) {
    auto* node = new Node<T>(val, head);
    head = node;
    ++count;
}

template<typename T>
LinkedList<T>& LinkedList<T>::operator+=(const LinkedList<T>& rhs) {
    LinkedList<T> copy = rhs;
    std::swap(head, copy.head);
    for (size_t i = 0; i < copy.size(); ++i) {
        push_tail(copy.at(i));
    }
    return *this;
}

template<typename T>
bool LinkedList<T>::pop_head(T& outValue) {
    if (head == nullptr) return false;

    Node<T>* toDelete = head;
    outValue = head->data;
    head = head->next;
    delete toDelete;
    --count;
    return true;
}

template<typename T>
bool LinkedList<T>::pop_tail(T& outValue) {
    if (head == nullptr || head->next == nullptr) {
        if (head != nullptr) {
            outValue = head->data;
            delete head;
            head = nullptr;
            --count;
            return true;
        }
        return false;
    }

    Node<T>* prev = head;
    Node<T>* tail = head->next;
    while (tail->next != nullptr) {
        prev = tail;
        tail = tail->next;
    }

    outValue = tail->data;
    prev->next = nullptr;
    delete tail;
    --count;
    return true;
}

template<typename T>
void LinkedList<T>::delete_node(const T& targetVal) {
    Node<T>* current = head;
    Node<T>* previous = nullptr;

    while (current != nullptr) {
        if (current->data == targetVal) {
            if (previous == nullptr) {
                head = current->next;
            }
            else {
                previous->next = current->next;
            }
            delete current;
            current = previous ? previous->next : head;
            --count;
        }
        else {
            previous = current;
            current = current->next;
        }
    }
}

template<typename T>
const T& LinkedList<T>::at(size_t index) const {
    if (index >= count) throw std::out_of_range("Index out of range");
    Node<T>* current = head;
    for (size_t i = 0; i < index; ++i) {
        current = current->next;
    }
    return current->data;
}

template<typename T>
T& LinkedList<T>::at(size_t index) {
    if (index >= count) throw std::out_of_range("Index out of range");
    Node<T>* current = head;
    for (size_t i = 0; i < index; ++i) {
        current = current->next;
    }
    return current->data;
}

template<typename T>
void LinkedList<T>::generate_random_elements(int seed, int numElements) {
    srand(seed);
    for (int i = 0; i < numElements; ++i) {
        push_tail(rand());
    }
}


template<typename T>
LinkedList<int> LinkedList<T>::generate_sequence(int n) {
    LinkedList<int> sequence;
    for (int i = 1; i <= n; ++i) {
        sequence.push_tail(i);
    }
    return sequence;
}


template<typename T>
void LinkedList<T>::remove_every_nth(int n) {
    if (n <= 1 || count == 0) return;

    int index = n - 1; 
    while (index < count) {
        
        if (index == 0) {
           
            Node<T>* temp = head;
            head = head->next;
            delete temp;
            count--;
        }
        else {
            
            Node<T>* current = head;
            for (int i = 0; i < index - 1; ++i) {
                current = current->next;
            }
            Node<T>* toDelete = current->next;
            current->next = toDelete->next;
            delete toDelete;
            count--;
        }

        
        index += n - 1;
    }
}

template<typename T>
LinkedList<int> LinkedList<T>::find_unlucky_numbers() {
    LinkedList<int> unlucky_numbers;
    LinkedList<int> temp = *this; 

    int step_index = 1; 

    while (step_index < temp.count && temp.count > 1) {
     
        int step = temp.at(step_index);

 
        int remove_index = step - 1; 

        while (remove_index < temp.count) {
            int value_to_remove = temp.at(remove_index);
            unlucky_numbers.push_tail(value_to_remove);
            remove_index += step;
        }

    
        temp.remove_every_nth(step);

      
        step_index++;
    }

    return unlucky_numbers;
}

int main() {
    LinkedList<int> myList;
    myList.generate_random_elements(56, 10);
    std::cout << "Original list: ";
    for (size_t i = 0; i < myList.size(); ++i) {
        std::cout << myList.at(i) << ' ';
    }
    std::cout << "\n";

    int removedHead;
    myList.pop_head(removedHead);
    std::cout << "Deleted from head: " << removedHead << "\n";

    int removedTail;
    myList.pop_tail(removedTail);
    std::cout << "Deleted from tail: " << removedTail << "\n";


    std::cout << "Sequence from 1 to 22:" << std::endl;
    LinkedList<int> sequence22 = LinkedList<int>::generate_sequence(22);

    LinkedList<int> unlucky_numbers = sequence22.find_unlucky_numbers();

    std::cout << "Unlucky numbers: ";
    for (size_t i = 0; i < unlucky_numbers.size(); ++i) {
        std::cout << unlucky_numbers.at(i) << " ";
    }
    

    return 0;
}