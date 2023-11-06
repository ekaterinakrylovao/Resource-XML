#include "XMLNode.h"
#include "XMLResource.h"

int main() {
    try {
        std::string filePath = "example.xml";
        auto xmlResource = XMLResource::Create(filePath);


        xmlResource->Print();

        XMLResource::iterator it = xmlResource->FindByTag("tag2");
        std::cout << "tag2 = " << it->getValue() << std::endl;

        std::cout << std::endl;

        it = xmlResource->FindByValue("3");
        std::cout << "3 in " << it->getTag() << " tag" << std::endl;


        XMLNode* element6 = new XMLNode("tag6", "6");
        it = xmlResource->FindByTag("tag4");
        xmlResource->Add(it, element6);


        it = xmlResource->FindByValue("3");
        xmlResource->Erase(it);


        xmlResource->Save("output.xml");
    }
    catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
    }

    return 0;
};