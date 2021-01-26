#include "logic.h"

namespace Logic {
    std::vector<NodeType> nodeTypes {
        NodeType(
            "base/goal", {tAny}, 
            [](Node *node, Object *obj) -> ValueType { return node->argNodes[0]->resolve(obj); }
        ),

        #define LR_OPERATOR(MATHOP) [](Node *node, Object *obj) -> ValueType {\
                                        return std::get<bool>(node->argNodes[0]->resolve(obj)) MATHOP std::get<bool>(node->argNodes[1]->resolve(obj));\
                                    }
        
        #define R_OPERATOR(MATHOP)  [](Node *node, Object *obj) -> ValueType {\
                                        return MATHOP std::get<bool>(node->argNodes[0]->resolve(obj));\
                                    }
        
        NodeType("math/add",      {{tInt, tFloat}, {tInt, tFloat}}, LR_OPERATOR(+)),
        NodeType("math/subtract", {{tInt, tFloat}, {tInt, tFloat}}, LR_OPERATOR(-)),
        NodeType("math/multiply", {{tInt, tFloat}, {tInt, tFloat}}, LR_OPERATOR(*)),
        NodeType("math/divide",   {{tInt, tFloat}, {tInt, tFloat}}, LR_OPERATOR(/)),
        NodeType("math/negate",   {{tInt, tFloat}},                  R_OPERATOR(-)),

        NodeType("gate/and", {{tBool}, {tBool}}, LR_OPERATOR(&&)),
        NodeType("gate/or",  {{tBool}, {tBool}}, LR_OPERATOR(||)),
        NodeType("gate/xor", {{tBool}, {tBool}}, LR_OPERATOR(^)),
        NodeType("gate/not", {{tBool}},           R_OPERATOR(!)),

        NodeType(
            "branch/if", {tAny, {tBool}},

            [](Node *node, Object *obj) -> ValueType {
                if (std::get<bool>(node->argNodes[1]->resolve(obj)))
                    return node->argNodes[0]->resolve(obj);
                else
                    return NullValue();
            }
        ),

        NodeType(
            "branch/while", {tAny, {tBool}},

            [](Node *node, Object *obj) -> ValueType {
                while (std::get<bool>(node->argNodes[1]->resolve(obj))) {
                    node->argNodes[0]->resolve(obj);
                }
                return NullValue();
            }
        ),

        NodeType(
            "branch/until", {tAny, {tBool}},

            [](Node *node, Object *obj) -> ValueType {
                while (!std::get<bool>(node->argNodes[1]->resolve(obj))) {
                    node->argNodes[0]->resolve(obj);
                }
                return NullValue();
            }
        ),
        
        #define NODE_CONSTANT [](Node *node, Object *obj) -> ValueType { return node->value; }
        NodeType("constant/bool",    {}, NODE_CONSTANT),
        NodeType("constant/int",     {}, NODE_CONSTANT),
        NodeType("constant/float",   {}, NODE_CONSTANT),
        NodeType("constant/null",    {}, NODE_CONSTANT),
        NodeType("constant/string",  {}, NODE_CONSTANT),
        NodeType("constant/vector2", {}, NODE_CONSTANT)
    };
}