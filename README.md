# TemplateLinkedList
Этот проект представляет собой реализацию односвязного списка (Linked List) на C++ с шаблонами, поддерживающую различные операции и алгоритмы обработки данных.

## Разберемся подробнее:
### 1. Структура Node
```cpp
template<typename T>
struct Node {
    T data;
    Node* next;

    explicit Node(const T& value = T(), Node* nex = nullptr): data(value), next(nex) {}
};
```
Это вспомогательная структура, при помощи которой мы создаем узел в списке. Узел состоит из хранимой информации и указателе на следующий узел. 

Так же здесь конструктор с параметрами по умолчанию, в котором у нас изначально указатель указывает на конец списка, а хранимое значание пустое.

Explicit - предотвращает неявные преобразования типов.
### 2. Конструктор копирования
```cpp
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
```
Это специальный конструктор, который создаёт полную независимую копию существующего списка. Без конструктора копирования компилятор создал бы его автоматически, но он скопировал бы только указатели (поверхностное копирование), что вызвало бы некорректную работу программы, а именно двойное удаление одной памяти.

Проверяем, пустой ли список. Если да, то выходим.

Далее копируем первый узел: выделяем новую память. Head теперь будет указывать на скопированный узел.

Циклом копируем все остальные узлы, идя дальше по списку от head и для каждого нового узла выделяем новую память.
### 3. Деструктор
```cpp
template<typename T>
LinkedList<T>::~LinkedList() {
    while (head != nullptr) {
        Node<T>* temp = head;
        head = head->next;
        delete temp;
    }
}
```
Это специальная функция, которая автоматически вызывается при уничтожении объекта LinkedList. Её задача — освободить все ресурсы, которые объект захватил за время жизни.

Пока список не пуст, мы заводим временный указатель на текущий узел и от head идем дальше по списку, но temp остается на предыдущем узле, его и удаляем. Далее temp будет указывать на второй и так далее узлы, удаляя их.
### 4. Оператор присваивания
```cpp
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
```
Это оператор, который позволяет присвоить один объект LinkedList другому.

Сначала мы проверяем не присваивается ли объект самому себе, а далее используется логика деструктора и конструктора копирования, описанная ранее. Но в данном случае, мы просто заменяем данные в списке, а не создаем новый с нуля.
### 5. Оператор сложения
```cpp
template<typename T>
LinkedList<T> LinkedList<T>::operator+(const LinkedList<T>& rhs) {
    LinkedList<T> result(*this);
    for (size_t i = 0; i < rhs.size(); ++i) {
        result.push_tail(rhs.at(i));
    }
    return result;
}
```
Мы создаем новый список, в котором сначала идут все элементы левого списка (*this), после них идут элементы правого списка (rhs), таким образом "складывая" два связных списка.
### 6. Метод добавления элемента в хвост списка
```cpp
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
```
Метод, который добавляет новый элемент с заданным значением в хвост связного списка.

Создаем новый узел, который содержит данные, которые как раз и хотим добавить в конец списка. Если список не пуст, то ищем последний узел. Как понять что мы в хвосте списка? Указатель next должен указывать на nullptr.

Новый созданный узел добавляем после хвоста. Увеличиваем количество элементов нашего списка.
### 7. Метод добавление элемента в голову списка
```cpp
template<typename T>
void LinkedList<T>::push_head(const T& val) {
    auto* node = new Node<T>(val, head);
    head = node;
    ++count;
}
```
Метод, который добавляет новый элемент с заданным значением в голову связного списка.

Создаем новый узел, который содержит данные, которые как раз и хотим добавить в начало списка. Но в данном случае нам не прийдется проходиться циклом по списку и искать конец, потому что изначально у нас итак указатель на head.

Добавляем новый узел в начало списка и не забываем, что количество элементов в списке изменилось.
### 8. Метод удаления элемента из конца списка
```cpp
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
```
Метод, который удаляет элемент из конца списка и возвращает его значение через параметр outValue.

Если в списке более 2 элементов, то заводим 2 указателя: на предпоследний элемент и последний. Когда tail указывает на nullprt, сохраняем значение в outValue и освобождаем память, теперь prev будет являться последним и указывать на nullprt, а счетчик элементов уменьшаем.

