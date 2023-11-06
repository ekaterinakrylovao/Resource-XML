#include "XMLNode.h"

XMLNode::XMLNode(const std::string& tag, const std::string& value) : tag(tag), value(value) {}

const std::string& XMLNode::getTag() const {
    return tag;
}

const std::string& XMLNode::getValue() const {
    return value;
}

void XMLNode::Add(std::unique_ptr<XMLNode> child) {
    children.push_back(std::move(child));
}

std::string XMLNode::Output(const int depth) {
    const std::string indent = std::string(depth, '\t');
    std::string result = "";

    result += indent + "<" + tag + ">" + value;
    if (children.size()) {
        result += "\n";
    }
    for (const auto& child : children) {
        result += child->Output(depth + 1);
    }
    if (children.size()) {
        result += indent;
    }
    result += "</" + tag + ">" + "\n";

    return result;
}

std::vector<XMLNode*> XMLNode::getDescendants() {
    std::vector<XMLNode*> descendants{};

    for (const auto& child : children) {
        descendants.push_back(child.get());
        std::vector<XMLNode*> child_descendants = child->getDescendants();
        descendants.insert(descendants.end(), child_descendants.begin(), child_descendants.end());
    }

    return descendants;
}

XMLNode::iterator XMLNode::Find(std::function<bool(XMLNode* node)> callback) {
    if (callback(this)) {
        return XMLNode::iterator(this);
    }
    for (const auto& child : children) {
        XMLNode::iterator node = child->Find(callback);
        if (*node != nullptr) {
            return node;
        }
    }

    return XMLNode::iterator(nullptr);
}

XMLNode::iterator::iterator(XMLNode* node) {
    if (node) {
        nodes = node->getDescendants();
        nodes.insert(nodes.begin(), node);
    }
    cur_node_index = 0;
}

bool XMLNode::iterator::operator == (const iterator& other) const {
    return **this == *other;
}

XMLNode* XMLNode::iterator::operator * () const {
    if (nodes.empty() || cur_node_index == nodes.size()) {
        return nullptr;
    }
    return nodes[cur_node_index];
}

XMLNode* XMLNode::iterator::operator -> () const {
    return **this;
}

XMLNode::iterator XMLNode::begin() {
    return XMLNode::iterator(this);
}

XMLNode::iterator XMLNode::end() {
    return XMLNode::iterator(nullptr);
}