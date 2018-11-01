#pragma once

#include <memory>
#include <unordered_map>
#include <ostream>

template <typename Char>
class Trie
{
public:
    Trie() : mRoot(std::make_unique<Node>())
    {

    }

    // Insertion

    template <typename String>
    void insert(String&& string)
    {
        Node* node = mRoot.get();
        for (auto&& c : string)
        {
            if (node->children.find(c) == node->children.end())
                node->children[c] = std::make_unique<Node>();
            node = node->children[c].get();
        }
        node->end = true;
    }

    // Overload for C strings
    template <typename T>
    void insert(T* string)
    {
        insert(std::basic_string<Char>(string));
    }

    // Membership

    template <typename String>
    bool contains(String&& string) const    
    {
        Node* node = mRoot.get();
        for (auto&& c : string)
        {
            if (node->children.find(c) == node->children.end())
                return false;
            node = node->children[c].get();
        }
        return node->end;

    }

    // Overload for C strings
    template <typename T>
    bool contains(T* string) const
    {
        return contains(std::basic_string<Char>(string));
    }

    // Prefix

    template <typename String>
    std::basic_string<Char> getLongestPrefix(String&& string) const
    {
        std::basic_string<Char> prefix;
        Node* node = mRoot.get();
        for (auto&& c : string)
        {
            if (node->children.find(c) == node->children.end())
                return prefix;
            prefix += c; // Maybe it could be optimized
            node = node->children[c].get();
        }
        return prefix;
    }

    // Overload for C strings
    template <typename T>
    std::basic_string<Char> getLongestPrefix(T* string) const
    {
        return getLongestPrefix(std::basic_string<Char>(string));
    }
    
    // Print

    std::ostream& print(std::ostream& os) const 
    {
        os << "root ";
        return print(os, mRoot.get(), "\t");
    }

private:
    struct Node
    {
        std::unordered_map<Char, std::unique_ptr<Node>> children;        
        bool end = false;
    };
    
    std::unique_ptr<Node> mRoot;

    // Print

    std::ostream& print(std::ostream& os, const Node* node, std::string tabs) const
    {
        os << node->end << '\n';
        for (const auto& kv : node->children)
        {
            os << tabs << kv.first << ' '; 
            print(os, kv.second.get(), tabs + '\t');
        }
        return os;
    }
};

template <typename Char>
std::ostream& operator<<(std::ostream& os, const Trie<Char>& trie)
{
    return trie.print(os);
}
