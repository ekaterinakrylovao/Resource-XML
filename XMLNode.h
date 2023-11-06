#pragma once
#include <iostream>
#include <string>
#include <vector>
#include <functional>

class XMLNode {
public:
    XMLNode(const std::string& tag, const std::string& value);

    const std::string& getTag() const;
    const std::string& getValue() const;

    std::vector<std::unique_ptr<XMLNode>> children;

    void Add(std::unique_ptr<XMLNode> child);
    std::string Output(const int depth = 0);
    std::vector<XMLNode*> getDescendants();

    class iterator {
    public:
        explicit iterator(XMLNode* node);

        bool operator == (const iterator& other) const;
        XMLNode* operator * () const;
        XMLNode* operator -> () const;

    private:
        std::vector<XMLNode*> nodes;
        int cur_node_index;
    };

    iterator begin();
    iterator end();
    iterator Find(std::function<bool(XMLNode* node)> callback);

private:
    std::string tag;
    std::string value;
};