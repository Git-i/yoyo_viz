#include <string_view>
constexpr std::string_view hl_query = R"(
("fn")@keyword.function 
[
    "for"
    "while"
]@keyword.repeat
[
    "struct" 
    "class" 
    "interface"
    "enum"
    "union"
] @keyword.type
[
    "return"
    "break"
    "continue"
]@keyword.return
(line_comment) @comment
(scope_expr (name_expr)@type)

(class_struct_decl .(identifier)@type)
(class_struct_decl (_) (identifier)@variable.memeber)

(enum_decl .(identifier)@type)
(enum_decl (_) (identifier)@constant)

(union_decl .(identifier)@type)
(union_decl (_) (identifier)@constant)

(function_decl .(identifier)@function)

(string)@string
(type (identifier)@type)
(type (primitivetype)@type)
(integer_literal)@number
)";
