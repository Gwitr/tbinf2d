#include "logic.h"
#include <stack>
#include <iostream>
#ifdef _WIN32
    #include <windows.h>
    #include <psapi.h>
#endif

namespace Logic {
    Error::Error(std::string s) {
        msg = s;
    }

    const char *Error::what() const noexcept {
        return msg.c_str();
    }

    NodeType::NodeType(std::string id, std::initializer_list<std::initializer_list<int>> argtypes, OnResolveType onResolve)
      : id{id}, onResolve{onResolve} {  
        for (auto il : argtypes) {
            this->argtypes.push_back(ArgTypes{il});
        }
    }
    ValueType Node::resolve(Object *as) {
        return nodeTypes[nodeType].onResolve(this, as);
    }
    Node Node::from_type(std::string s) {
        int index = 0;
        for (auto nodeType : nodeTypes) {
            if (nodeType.id == s) {
                Node result;
                result.nodeType = index;
                result.value = NullValue();
                return result;
            }
            index++;
        }
        std::string emsg = "The node type '";
        emsg += s;
        emsg += "' doesn't exist.";
        throw Error(emsg);
    }
    void Node::add_input(Node *n) {
        argNodes.push_back(n);
        n->outNodes.push_back(this);
    }
}