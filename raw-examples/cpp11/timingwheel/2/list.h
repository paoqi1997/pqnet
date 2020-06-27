#ifndef LIST_H
#define LIST_H

template <typename T>
struct LinkNode
{
    LinkNode() : prev(nullptr), next(nullptr) {}
    LinkNode(T _element) : prev(nullptr), next(nullptr), element(_element) {}
    ~LinkNode() {
        if (prev != nullptr) {
            prev->next = next;
        }
        if (next != nullptr) {
            next->prev = prev;
        }
    }
    LinkNode<T> *prev;
    LinkNode<T> *next;
    T element;
};

template <typename T>
class List
{
public:
    List() : head(new LinkNode<T>()), tail(new LinkNode<T>())
    {
        head->next = tail;
        tail->prev = head;
    }
    ~List() {
        auto p = head->next;
        while (p != tail) {
            auto q = p->next;
            delete p;
            p = q;
        }
        delete head;
        delete tail;
        head = tail = nullptr;
    }
    LinkNode<T>* push_back(T element) {
        auto node = new LinkNode<T>(element);
        auto p = tail->prev;

        // prev部分
        tail->prev = node;
        node->prev = p;
        // next部分
        p->next = node;
        node->next = tail;

        return node;
    }
    LinkNode<T>* begin() { return head->next; }
    LinkNode<T>* end() { return tail; }
private:
    LinkNode<T> *head;
    LinkNode<T> *tail;
};

#endif // LIST_H
