#pragma once

template<typename KeyType, typename ValueType>
class MyMap
{
public:
    MyMap()
    {
        size_ = 0;
        root_ = nullptr;
    }

    ~MyMap()
    {
        recursive_clear(root_);
    }

    int size() const
    {
        return size_;
    }

    void associate(const KeyType& key, const ValueType& value)
    {
        if (root_ == nullptr) {
            root_ = new Node(key, value);
            size_++;
            return;
        }

        Node* current = root_;
        for (;;) {
            if (current->first_ == key) {
                current->second_ = value;
                return;
            }
            else if (key < current->first_) {
                if (current->left_ != nullptr) {
                    current = current->left_;
                }
                else {
                    current->left_ = new Node(key, value);
                    size_++;
                    return;
                }
            }
            else if (key > current->first_) {
                if (current->right_ != nullptr) {
                    current = current->right_;
                }
                else {
                    current->right_ = new Node(key, value);
                    size_++;
                    return;
                }
            }
        }
    }

    // For a map that can't be modified, return a pointer to const ValueType
    ValueType* find(const KeyType& key) const
    {
        Node* current = root_;
        while (current != nullptr) {
            if (current->first_ == key) return &current->second_;
            if (key < current->first_) {
                current = current->left_;
            }
            else {
                current = current->right_;
            }
        }

        return nullptr;
    }

    // For a modifiable map, return a pointer to modifiable ValueType
    ValueType* find(const KeyType& key)
    {
        return const_cast<ValueType*>(const_cast<const MyMap*>(this)->find(key));
    }

private:
    struct Node
    {
        Node(KeyType first, ValueType second)
        {
            first_ = first;
            second_ = second;
            left_ = right_ = nullptr;
        }
        Node(KeyType first, ValueType second, Node* left_child, Node* right_child)
        {
            first_ = first;
            second_ = second;
            left_ = left_child;
            right_ = right_child;
        }

        KeyType first_;
        ValueType second_;
        Node* left_;
        Node* right_;
    };

    Node* root_;
    int size_;

    void recursive_clear(Node* root_)
    {
        if (root_ == nullptr) return;
        recursive_clear(root_->left_);
        recursive_clear(root_->right_);
        delete root_;
    }
};
