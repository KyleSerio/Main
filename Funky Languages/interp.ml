(*MiniBasic Interpreter, using OCaml
  Written by Kyle Serio. All professor provided code has been removed*)
open Absyn
open Tables

let want_dump = ref false

let source_filename = ref ""

(*Table of accepted unary operands*)
let unop oper a : float = match oper with
    | "+" -> +.a
    | "-" -> -.a
    | "log" -> log a
    | "log10" -> log10 a
    | "sqrt" -> sqrt a
    | "exp" -> exp a
    | "sin" -> sin a
    | "cos" -> cos a
    | "tan" -> tan a
    | "asin" -> asin a
    | "acos" -> acos a
    | "atan" -> atan a
    | "abs" -> abs_float a
    | "ceil" -> ceil a
    | "floor" -> floor a
    | "round" -> floor (a +. 0.5)
    | _ -> a

(*Table of accepted binary operands*)
let binop oper a b : float = match oper with
    | "+" -> a +. b
    | "-" -> a -. b
    | "*" -> a *. b
    | "/" -> if (a = 0.0 && b = 0.0) then nan
             else if (a == +.1.0 && b == +.0.0) then infinity
             else if (a == +.1.0 && b == -.0.0) then neg_infinity
             else if (a == -.1.0 && b == -.0.0) then infinity
             else if (a == -.1.0 && b == +.0.0) then neg_infinity
             else a /. b
    | "^" -> a ** b
    | _ -> nan

(*Handles expressions, including values, variables and the associated 
  lookup, and mathematic expressions to any depth*)
let rec eval_expr (expr : Absyn.expr) : float = match expr with
    | Number number -> number
    | Memref memref -> eval_memref memref
    | Unary (oper, expr) -> unop oper (eval_expr expr)
    | Binary (oper, expr1, expr2) -> binop oper 
                                    (eval_expr expr1) (eval_expr expr2)

(*Handles returning the value of a variable from the storage table*)
and eval_memref (memref : Absyn.memref) : float = match memref with
    | Arrayref (ident, expr) -> 
                (try Array.get 
                    (Hashtbl.find Tables.array_table ident)
                    (Float.to_int (floor((eval_expr expr) +. 0.5)))
                with Not_found -> 0.0)
    | Variable ident -> 
               (try Hashtbl.find Tables.variable_table ident
               with Not_found -> 0.0)

(*Handles program flow and continuations*)
let rec interpret (program : Absyn.program) = match program with
    | [] -> ()
    | firstline::continue -> match firstline with
       | _, _, None -> interpret continue
       | _, _, Some stmt -> (interp_stmt stmt continue)

(*Simple switchboard for each statement type*)
and interp_stmt (stmt : Absyn.stmt) (continue : Absyn.program) =
    match stmt with
    | Dim (ident, expr) -> interp_dim ident expr continue
    | Let (memref, expr) -> interp_let memref expr continue
    | Goto label -> interp_goto label continue
    | If (expr, label) -> interp_if expr label continue
    | Print print_list -> interp_print print_list continue
    | Input memref_list -> interp_input memref_list continue

(*Handles if statements*)
and interp_if (expr : Absyn.relexpr) label continue = match expr with
    | Relexpr (oper, expr1, expr2) -> 
        if (eval_rel oper (eval_expr expr1) (eval_expr expr2)) = true 
        then interp_goto label continue
        else interpret continue                                 
                                      
(*Handles relational operations*)
and eval_rel oper expr1 expr2 = match oper with
    | ">" -> expr1 > expr2
    | ">=" -> expr1 >= expr2
    | "<" -> expr1 < expr2
    | "<=" -> expr1 <= expr2
    | "!=" -> expr1 <> expr2
    | "=" -> expr1 = expr2
    | _ -> false

(*Handles creation of arrays*)
and interp_dim ident expr continue =
    Hashtbl.replace Tables.array_table ident 
    (Array.make (Float.to_int (floor ((eval_expr expr) +. 0.5))) 0.0);
    interpret continue

(*Handles program execution and control jumps*)
and interp_goto label continue =
    try interpret (Hashtbl.find Tables.label_table label)
    with Not_found -> print_string "Label not Found!\n"

(*Handles variable value assignment*)
and interp_let (memref : Absyn.memref) (expr1 : Absyn.expr) continue = 
    match memref with
    | Arrayref (ident, expr) -> 
                if 
                Array.length (Hashtbl.find Tables.array_table ident) <=
                        (Float.to_int (floor((eval_expr expr) +. 0.5)))
                then print_string "Index out of bounds!\n"
                else
                Array.set 
                (Hashtbl.find Tables.array_table ident) 
                (Float.to_int (floor((eval_expr expr) +. 0.5)))
                (eval_expr expr1);
    interpret continue
    | Variable ident -> 
                Hashtbl.replace 
                Tables.variable_table 
                ident (eval_expr expr1);
    interpret continue


(*Handles user input*)
let interp_input (memref_list : Absyn.memref list)
                 (continue : Absyn.program)  =
    let input_number memref = match memref with
        | Arrayref (ident, expr) -> 
                    print_string "Unneeded!"
        | Variable (ident) ->
            if (Hashtbl.find Tables.variable_table "eof") != 1.0 then
            (
                try  let number = Etc.read_number ()
                    in (Hashtbl.replace 
                        Tables.variable_table 
                        ident number;)
                with End_of_file -> 
                    (Hashtbl.replace 
                    Tables.variable_table 
                    "eof" 1.0;)
            )
    in List.iter input_number memref_list;
    interpret continue
