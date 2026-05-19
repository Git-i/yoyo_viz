#include <string_view>
// we separate these queries becuase they override previous highlights
constexpr std::string_view call_queries = R"(
(call_expr .(expression (binary_expr (expression) (expression (name_expr))@function)))
(call_expr .(expression (binary_expr (expression) (expression (scope_expr (name_expr)@function .)))))
(call_expr .(expression (scope_expr (name_expr)@function .)))

(str_escape)@str_escape
(str_capture "${"@str_cap "}"@str_cap)
)";
constexpr std::string_view hl_query = R"--(
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
(class_struct_decl (_) (identifier)@variable.member)

(enum_decl .(identifier)@type)
(enum_decl (_) (identifier)@constant)

(union_decl .(identifier)@type)
(union_decl (_) (identifier)@constant)

(function_decl .(identifier)@function)

(str_text)@string
(string "\""@string)
(type (identifier)@type)
(type (primitivetype)@type)
(integer_literal)@number
(float_literal)@number

(obj_literal (name_expr)@type)
(obj_literal (identifier)@variable.member)
(type (generic_name_expr (identifier)@type))
(generic_clause (identifier)@type)

"this"@variable.builtin

(domain_annot)@domain
(domain_list_item)@domain

(bool_literal)@boolean
(null_literal)@null
(cast_expr "as"@keyword.modifier)

(binary_expr (_) _@operator (_))
(var_decl ":"@operator "="@operator)
";"@delim
(prefix_expr ._@operator (_))
[
    "("
    ")"
    "{"
    "}"
]@delim

(binary_expr (expression) "." (expression (name_expr))@variable.member)
(call_expr (expression (name_expr))@function)
(if_expr "if"@keyword.conditional) (if_expr "else"@keyword.conditional)
(cond_extract_expr "if"@keyword.conditional) (cond_extract_expr "else"@keyword.conditional)
)--";

