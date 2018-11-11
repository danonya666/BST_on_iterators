//Some attributes have been moved from private to public in order to test.
#include <iostream>
#include <iterator>

using namespace std;

template <typename T>
struct Node
{
    T value;
    Node* left;
    Node* right;
    Node* parent;
};

template <typename Node>
class bstIter : public std::iterator<std::input_iterator_tag, Node>
{
    template <typename T>
    friend class bst;
private:
    Node* root;
public:
    bstIter(Node* root, Node* p);
    bstIter();
    Node* p;
    bstIter(const bstIter &it);
    bool operator!=(bstIter const& other) const;
    bool operator==(bstIter const& other) const;
    typename bstIter::reference operator*() const;
    bstIter& operator++();
    bool has_children();
    bstIter& operator=(bstIter& other);
    Node* son();
};

template<typename Node>
bstIter<Node>::bstIter(Node* root, Node* p) :
    p(p), root(root)
{

}


template<typename Node>
bstIter<Node>::bstIter()
{
    root = nullptr;
    p = nullptr;
}

template<typename Node>
bstIter<Node>::bstIter(const bstIter& it) :
    p(it.p), root(it.root)
{

}

template<typename Node>
bool bstIter<Node>::operator!=(bstIter const& other) const
{
    return p != other.p;
}

template<typename Node>
bool bstIter<Node>::operator==(bstIter const& other) const
{
    return p == other.p;
}

template<typename Node>
typename bstIter<Node>::reference bstIter<Node>::operator*() const
{
    return *p;
}

template<typename Node>
bstIter<Node> &bstIter<Node>::operator++()
{
    if(p->left != nullptr)
    {
        p = p->left;
        return *this;
    }
    else if(p->right != nullptr)
    {
        p = p->right;
        return *this;
    }
    else if(p->parent != nullptr)
    {
        Node* n1;
        Node* n2;
        n1 = p;
        n2 = p->parent;
        if(n2->left == n1)
        {
            while(n2->right == nullptr && n2->parent != nullptr)
            {
                n1 = n2;
                n2 = n2->parent;
            }
        }
        if (n2->right != nullptr)
        {
            p = n2->right;
            return *this;
        }


        while(n2->right == n1 && n2 != root)
        {
            n1 = n2;
            n2 = n2->parent;
        }
        if(n2 == root && n2->right == n1)
        {
            1;
        }
        else
        {
            p = n2->right;
        }
    }
    return *this;
}

template<typename Node>
bool bstIter<Node>::has_children()
{
    return (this->p->left != nullptr || this->p->right != nullptr);
}

template<typename Node>
bstIter<Node>& bstIter<Node>::operator=(bstIter& other)
{
    this->p = other.p;
}

template<typename Node>
Node* bstIter<Node>::son()
{
    return((this->p->left != nullptr)? this->p->left:this->p->right);
}


template <typename T>
class bst
{
    typedef Node<T> Node;
private:

public:
    Node* root;
    typedef bstIter <Node> iterator;
    typedef bstIter <const Node> const_iterator;

    bst()
    {
        root = nullptr;
    }

    bst(const bst& other): root(other.root)
    {

    }

    iterator begin()
    {
        iterator it(root, root);
        return it;
    }

    const_iterator cbegin()
    {
        const_iterator cit(root, root);
        return cit;
    }

    iterator end()
    {
        iterator result = this->begin();
        for(iterator check = result; ;check = result)
        {
            ++result;
            if(result == check)
                break;
        }
        return result;
    }

    const_iterator cend()
    {
        const_iterator result = this->cbegin();
        for(const_iterator check = result; ;check = result)
        {
            ++result;
            if(result == check)
                break;
        }
        return result;
    }


    iterator insert(const T& value)
    {
        bool flag = 0;
        iterator curr;
        curr.p = root;
        if(curr.p == nullptr)
        {
            Node* n = new Node;
            n->left = nullptr;
            n->right = nullptr;
            n->parent = nullptr;
            n->value = value;
            flag = 1;
            root = n;
        }
        else do
        {
            if(value > curr.p->value)
            {
                if(curr.p->right == nullptr)
                {
                    Node* n = new Node;
                    n->left = nullptr;
                    n->right = nullptr;
                    n->parent = curr.p;
                    n->value = value;
                    curr.p->right = n;
                    flag = 1;
                    return curr;
                }
                else
                {
                    curr.p = curr.p->right;
                }
            }
            else if(value == curr.p->value)
            {
                cout << "Node " << value << " already exsists\n";
                return curr;
            }

            else if(value < curr.p->value)
            {
                if(curr.p->left == nullptr)
                {
                    Node* n = new Node;
                    n->left = nullptr;
                    n->right = nullptr;
                    n->parent = curr.p;
                    n->value = value;
                    curr.p->left = n;
                    flag = 1;
                    return curr;
                }
                else
                {
                    curr.p = curr.p->left;
                    continue;
                }
            }

        }
        while(!flag);
    }

    iterator find(const T& value)
    {
        iterator result;
        result.p = nullptr;
        iterator curr;
        curr.p = root;
        //cout << "!!" << root;
        if(root != nullptr)
        {
            while(curr.has_children())
            {
                //cout << 4;
               // cout << "@";
                if(value > curr.p->value)
                {
                    curr.p = curr.p->right;
                }
                else if(value < curr.p->value)
                {
                    curr.p = curr.p->left;
                }
                else if(value == curr.p->value)
                {
                    return curr;
                }
            }
        }

        //cout << "%$";
        //cout << endl << "!!!" << (curr.p == nullptr) << endl;
        return curr;
    }

    iterator remove(const T& value)
    {
        iterator found = find(value);
        if(found.p == nullptr)
        {
            cout << "Nothing to remove\n";
        }
        else if(!found.has_children())
        {
            if(found.p->parent != nullptr)
            {

                if(found.p->value < found.p->parent->value)
                {
                    found.p->parent->left = nullptr;
                    found.p = nullptr;
                }
                else
                {
                    found.p->parent->right = nullptr;
                    found.p = nullptr;
                }
            }
        }
        else if(found.has_children() && (found.p->left == nullptr || found.p->right == nullptr))
        {
            if(found.p->parent != nullptr)
            {
                //cout << 8;
                if(found.p->value < found.p->parent->value)
                {
                    found.p->parent->left = found.son();
                    found.p = nullptr;
                }
                else
                {
                    found.p->parent->right = found.son();
                    found.p = nullptr;
                }
            }
            else
            {
                root = found.son();
                found.p = nullptr;
            }
        }
        else
        {
            Node* sub;
            T subValue;
            iterator found1 = found;
            if(found1.p->left == nullptr)
                found1.p = found1.p->right;
            while(found1.p->left != nullptr)
                found1.p = found1.p->left;
            subValue = found1.p->value;
            remove(subValue);
            found.p->value = subValue;
        }
    }

    const_iterator find(const T& value) const
    {
        const_iterator result;
        result.p = nullptr;
        const_iterator curr;
        curr.p = root;
        while(curr.has_children())
        {
            if(value > curr.p->value)
            {
                curr.p = curr.p->right;
            }
            else if(value < curr.p->value)
            {
                curr.p = curr.p->left;
            }
            else if(value == curr.p->value)
            {
                return curr;
            }
        }

        return curr;
    }

    bool empty() const
    {
        return(root == nullptr);
    }

    size_t size() const
    {
        iterator check;
        iterator curr;
        size_t counter = 0;
        for(check = curr; ; check = curr)
        {
            ++curr;
            counter++;
            if(curr == check)
                break;
        }
        return counter;
    }
};
int main()
{
    typedef bstIter <Node<int>> iterator;
    bst<int> tree;
    tree.insert(5);
    tree.insert(6);
    tree.insert(7);
    tree.insert(4);
    auto it = tree.begin();
    auto eind = tree.end();
    cout << it.p->value;
    ++it;
    cout << it.p->value;
    ++it;
    cout << it.p->value;
    ++it;
    cout << it.p->value;
    ++it;
    cout << it.p->value;
    tree.remove(5);
    return 0;
}
