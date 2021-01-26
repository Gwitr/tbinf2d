#ifndef LOGIC_H
#define LOGIC_H

#include <variant>
#include <vector>
#include <string>
#include <functional>
#include <unordered_set>

#include "point.h"

class Object;

namespace Logic {
    // Value type definitions
    typedef std::monostate NullValue;
    static constexpr int tNull    = 0;
    static constexpr int tInt     = 1;
    static constexpr int tFloat   = 2;
    static constexpr int tBool    = 3;
    static constexpr int tString  = 4;
    static constexpr int tVector2 = 5;
    static constexpr int tObject  = 6;
    inline std::initializer_list<int> tAny = {Logic::tNull, Logic::tInt, Logic::tFloat, Logic::tBool, Logic::tString, Logic::tVector2, Logic::tObject};
    typedef std::variant<NullValue, int, float, bool, std::string, Point, Object *> ValueType;

    // A single logic node
    struct Node;
    struct Node {
        unsigned int nodeType;
        std::vector<Node *> argNodes;
        std::vector<Node *> outNodes;

        ValueType value;

        ValueType resolve(Object *);
        void add_input(Node *);

        static Node from_type(std::string);
    };

    // NodeType types
    typedef std::unordered_set<int> ArgTypes;
    typedef std::vector<ArgTypes> NodeTypeArgs;
    typedef std::function<ValueType(Node *, Object *)> OnResolveType;

    // Class describing a logic node type
    struct NodeType {
        std::string id;
        NodeTypeArgs argtypes;
        OnResolveType onResolve;
        
        NodeType(std::string id, NodeTypeArgs argtypes, OnResolveType onResolve);
        NodeType(std::string id, std::initializer_list<std::initializer_list<int>> argtypes, OnResolveType onResolve);
    };

    extern std::vector<NodeType> nodeTypes;

    class Error : public std::exception {
        public:
        Error(std::string);
        const char *what() const noexcept;

        private:
        std::string msg;
    };
}
#endif