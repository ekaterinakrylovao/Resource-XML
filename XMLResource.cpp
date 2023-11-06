#include "XMLResource.h"

XMLResource::XMLResource(const std::string& path) {
    XMLResource::Load(path);
}

std::unique_ptr<XMLResource> XMLResource::Create(const std::string& path) {
	return std::make_unique<XMLResource>(path);
}

void XMLResource::Load(const std::string& path) {
    std::ifstream file(path);

    if (!file) {
        throw std::runtime_error("File not found.");
    }

    const std::string XML = std::string((std::istreambuf_iterator<char>(file)), std::istreambuf_iterator<char>());
    Parse(XML);
}

void XMLResource::Print() {
    std::string XML = Output();
    std::cout << XML << std::endl;
}

void XMLResource::Parse(const std::string& XML) {
    int pos = 0;
    root_node = ParseNode(XML, pos);
}

void XMLResource::Save(const std::string& path) {
    std::string XML = Output();
    std::ofstream file(path);

    if (!file) {
        throw std::runtime_error("File not found.");
    }

    file << XML;
}

std::string XMLResource::ExtractValue(const std::string& str) {
    auto isNotSpace = [](char c) { return !std::isspace(c); };
    auto front = std::find_if(str.begin(), str.end(), isNotSpace);
    auto back = std::find_if(str.rbegin(), str.rend(), isNotSpace).base();

    return (back <= front) ? std::string() : std::string(front, back);
}

std::unique_ptr<XMLNode> XMLResource::ParseNode(const std::string& XML, int& pos) {
    pos = XML.find("<", pos) + 1;
    int end = XML.find(">", pos);
    std::string tag = XML.substr(pos, end - pos);
    int start = pos = end + 1;
    pos = XML.find("<", pos);
    std::string value = ExtractValue(XML.substr(start, pos - start));
    std::unique_ptr<XMLNode> node(new XMLNode(tag, value));

    pos = XML.find("<", pos) + 1;
    end = XML.find(">", pos);
    std::string nextTag = XML.substr(pos, end - pos);
    pos = end + 1;
    while (nextTag != ("/" + tag) && pos < XML.size()) {
        pos -= nextTag.size() + 2;

        if (pos < 0) {
            throw std::runtime_error("Incorrect XML format.");
        }

        node->Add(ParseNode(XML, pos)); 
        pos = XML.find("<", pos) + 1;
        end = XML.find(">", pos);
        nextTag = XML.substr(pos, end - pos);
        pos = end + 1;
    }

    return node;
}

std::string XMLResource::Output() {
    return root_node->Output();
}

XMLNode::iterator XMLResource::Add(XMLNode::iterator it, XMLNode* node) {
    if (!*it) {
        throw std::runtime_error("Node does not exist.");
    }

    it->Add(std::unique_ptr<XMLNode>(node));

    return XMLNode::iterator(node);
}

XMLNode::iterator XMLResource::FindByTag(const std::string& tag) {
    return root_node->Find([&tag](XMLNode* node) {
        return node->getTag() == tag;
        });
}

XMLNode::iterator XMLResource::FindByValue(const std::string& value) {
    return root_node->Find([&value](XMLNode* node) {
        return node->getValue() == value;
        });
}

bool XMLResource::Erase(XMLNode::iterator it) {
    if (*it == root_node.get()) {
        throw std::logic_error("Cannot delete the root node.");
    }

    auto parent_it = root_node->Find([target = it.operator->()](XMLNode* node) {
        auto vec_it = std::find_if(
            node->children.begin(),
            node->children.end(),
            [target](const std::unique_ptr<XMLNode>& ptr) {
                return ptr.get() == target;
            }
        );

        return vec_it != node->children.end();
        });

    if (parent_it == root_node->end()) {
        return false;
    }

    parent_it->children.insert(
        parent_it->children.end(),
        std::make_move_iterator(it.operator->()->children.begin()),
        std::make_move_iterator(it.operator->()->children.end())
    );

    auto vec_it = std::find_if(
        parent_it->children.begin(),
        parent_it->children.end(),
        [target = it.operator->()](const std::unique_ptr<XMLNode>& ptr) {
            return ptr.get() == target;
        }
    );

    if (vec_it != parent_it->children.end()) {
        parent_it->children.erase(vec_it);
        return true;
    }

    return false;
}