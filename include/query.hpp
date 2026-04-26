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
    "operator"
] @keyword.type
[
    "return"
    "break"
    "continue"
]@keyword.return
[
    "mut"
]@keyword.modifier
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
(float_literal)@number

(obj_literal (name_expr)@type)
(type (generic_name_expr (identifier)@type))
(generic_clause (identifier)@type)

"this"@variable.builtin

(domain_annot)@domain
(domain_list_item)@domain

(call_expr .(expression (binary_expr (expression) (expression (name_expr))@function)))
)";
