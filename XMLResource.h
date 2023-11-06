#pragma once
#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include "XMLNode.h"

class XMLResource {
public:
    XMLResource(const std::string& path);
    static std::unique_ptr<XMLResource> Create(const std::string& path);

    void Load(const std::string& path);
    void Print();
    void Parse(const std::string& XML);
    void Save(const std::string& path);

    using iterator = XMLNode::iterator;

    XMLNode::iterator Add(XMLNode::iterator it, XMLNode* node);
    XMLNode::iterator FindByTag(const std::string& tag);
    XMLNode::iterator FindByValue(const std::string& value);
    bool Erase(XMLNode::iterator it);

private:
    std::unique_ptr<XMLNode> root_node;

    std::string ExtractValue(const std::string& str);
    std::unique_ptr<XMLNode> ParseNode(const std::string& XML, int& pos);
    std::string Output();
};