#ifndef TREEMULTIMAP_INCLUDED
#define TREEMULTIMAP_INCLUDED

#include <iostream>
#include <vector>
using namespace std;

template <typename KeyType, typename ValueType>
class TreeMultimap
{
public:
    // This is the iterator that goes through the Values associated with a given Key
    class Iterator
    {
    public:
        Iterator() // Needs to be invalid so it can be checked by is_valid() and so we can return invalid iterators
        {
            m_is_valid = false;
            m_index = 0;
            m_ValueType_vector = nullptr;
        }

        Iterator(std::vector<ValueType>& ValueType_vector)
        {
            m_is_valid = true;
            m_index = 0;
            m_ValueType_vector = &ValueType_vector;
        }

        ValueType& get_value() const
        {
            return (*m_ValueType_vector)[m_index];
        }

        bool is_valid() const
        {
            return m_is_valid;
        }

        void advance()
        {
            m_index++;
            if (m_index >= m_ValueType_vector->size())
                m_is_valid = false;
        }

    private:
        bool m_is_valid;
        int m_index;
        std::vector<ValueType>* m_ValueType_vector;
    };

    TreeMultimap()
    {
        m_root_pointer = nullptr;
    }

    ~TreeMultimap()
    {
        delete_binary_tree_helper(m_root_pointer);
    }

    void insert(const KeyType& key, const ValueType& value)
    {

        if (m_root_pointer == nullptr)
        {
            m_root_pointer = new binary_tree_node(key, value);
            return;
        }

        binary_tree_node* current = m_root_pointer;
        for (;;)
        {
            // If it's the same as the key, we'll simply push_back a value to its vector
            if (key == current->node_key)
            {
                current->vector_associated_with_key.push_back(value);
                return;
            }

            // If it's less than the key
            else if (key < current->node_key)
            {
                // If a left node already exists, then we'll just traverse to the left
                if (current->left != nullptr)
                    current = current->left;
                // Otherwise we'll add a new left node there
                else
                {
                    current->left = new binary_tree_node(key, value);
                    return;
                }
            }

            else
            {
                if (current->right != nullptr)
                    current = current->right;
                else
                {
                    current->right = new binary_tree_node(key, value);
                    return;
                }
            }
        }
    }

    // Must be log n speed
    Iterator find(const KeyType& key) const
    {
        binary_tree_node* pointer_to_found_node = search_tree(key, m_root_pointer);

        if (pointer_to_found_node == nullptr)
        {
            Iterator invalid_iterator;
            return invalid_iterator;
        }

        else
        {
            Iterator valid_iterator(pointer_to_found_node->vector_associated_with_key);
            return valid_iterator;
        }
    }

private:
    struct binary_tree_node
    {
        binary_tree_node(const KeyType& key, const ValueType& value)
        {
            node_key = key;
            vector_associated_with_key.push_back(value);
            left = right = nullptr;
        }

        KeyType node_key;
        std::vector<ValueType> vector_associated_with_key;
        binary_tree_node* left;
        binary_tree_node* right;
    };

    binary_tree_node* m_root_pointer;

    binary_tree_node* search_tree(const KeyType& key, binary_tree_node* pointer_to_node) const
    {
        if (pointer_to_node == nullptr)
            return nullptr;
        else if (key == pointer_to_node->node_key)
            return pointer_to_node;
        else if (key < pointer_to_node->node_key)
            return search_tree(key, pointer_to_node->left);
        else
            return search_tree(key, pointer_to_node->right);
    }

    void delete_binary_tree_helper(binary_tree_node* pointer_to_node)
    {
        // Post order deletion so we can delete all the leaves first

        if (pointer_to_node == nullptr)
            return;

        delete_binary_tree_helper(pointer_to_node->left);
        delete_binary_tree_helper(pointer_to_node->right);
        delete pointer_to_node;
    }
};

#endif // TREEMULTIMAP_INCLUDED
