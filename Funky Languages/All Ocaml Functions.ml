(* $Id: interp.ml,v 1.19 2021-04-20 20:19:30-07 - - $ *)
(*random facts - 
reserved keywords : https://ocaml.org/manual/lex.html - scroll/ctrl-f down
comments considered blank space, ignored except to separate identifiers/code
parsing - verify tokens are in correct syntactic order, and assemble abstract syntax
_::[] means the same as [_]
strong and dynamic type check - scheme and smalltalk
ocaml differentiates between lower/uppercase (most? all? situations)
underscore _ considered lowercase 
rec - recursive let (let vs. let rec) extends scope of data to the recursive calls as well
single quote - 'a means a generic type a, while a' is just an identifier, a' differs from a/aa etc, all variable names
:: - cons operator, so 1 :: 2 makes a list [1;2], 3 :: [1;2] makes [3;1;2] --**---DO ELEM::LIST NOT LIST::ELEM--**---
arrow -> syntax : https://stackoverflow.com/questions/42816588/what-is-the-purpose-of-an-arrow-in-ocaml
^ - append, so "ab" ^ "c" = "abc"
remember: fold - collapses a list to a single value!
remember: map - if input is a list, map will apply something to each elemenet, and return a list
types - multiply ( * ) = int->int->int - since it takes an int and an int to mult, and returns an int
*)


(*collatz function*)
let collatz n =
    let rec collatz' n rest =
        if n <= 1
           then 1::rest
           else if n mod 2 = 0
                   then collatz' (n / 2) (n::rest)
                   else collatz' (n * 3 + 1) (n::rest)
    in  List.rev (collatz' n [])

(*sum function - no higher order - from solutions*)
let sum list = 
    let rec sum' list' n = match list' with
        | [] -> n (*if list' is empty, return n which is used as running sum *)
        | x::xs -> sum' xs (x + n) (* x::xs ??? = recurse with xs (?) and x + n where n is the new running sum, and xs is the remaining list?*)
    in sum' list 0 (*outer start to the recursive func, starts with the given list and a sum of 0 *)

(*Defining define fold left foldl*)
let rec fold_left fn z list = match list with
    | [] -> z
    | x::xs -> fold_left fn (fn z x) xs (*fn (inner fn call on car) (rest of list) *)

(*sum using fold foldl *)
let sum list = fold_left (+) 0 list (*  OR  *) let sum = fold_left (+) 0

(*define grep - given a predicate and a list, returns all elements where the predicate is true*)
let rec grep p list = match list with
    | [] -> []
    | x::xs when p x -> x :: grep p xs
    | _::xs -> grep p xs

(*finds all odd positive integers using grep *)
let oddpos = grep (fun x -> x > 0 && x mod 2 = 1)

(*gcd -  the greatest common denominator of two given numbers *)
let rec gcd x y =
    if x > y then gcd (x - y) y
    else if x < y then gcd x (y - x)
    else x;;

(*factorial - fail for negative number *)
let fac n =
    let rec fac' n' a' =
        if n' <= 1 then a'
                   else fac' (n' - 1) (n' * a')
    in if n < 0 then failwith "fac n | n < 0"
                else fac' n 1

(*odd length - determines if a list has an odd length *)
let rec oddlen list = match list with
    | [] -> false
    | [_] -> true
    | _::_::xs -> oddlen xs

(*eval expr evalute expression eval_expr -- Uses these declarations: 
type binfn = float -> float -> float
type expr = Expr of binfn * expr * expr
          | Num of float*)
let rec eval expr = match expr with
    | Num value -> value (*base case, return the value *)
    | Expr (binfn, e1, e2) -> binfn (eval e1) (eval e2) (*binary func,  recurse(?) with e1 and e2 *)

(*reverse a list *)
let reverse list =            
    let rec rev inl outl = match inl with (* define recursive func *)
        | [] -> outl (*return the reversed portion *)
        | x::xs -> rev xs (x::outl) (* xs = rest of list, (x::outl) = cons the cur element with the reversed portion, pass as reversed portion again *)
    in rev list [] (* outer func call *)

(* length of a list, no higher order functions *)
let length list =
    let rec len lst acc = match lst with
        | [] -> acc (*done, return accumulator *)
        | _::xs -> len xs (acc + 1) (* theres more! recurse with rest of list (w/o first elem), and acc + 1 *)
    in len list 0

(*simply drops/removes the first n elements of the list *)
let rec drop1 n list = match list with
| [] -> []
| _::xs when n > 0 -> drop (n - 1) xs
| _ -> list ;;

(*predicate -finds the first pair in a list of key/val pairs that match the given predicate, and returns the matching value *)
let rec findpred p list = match list with
    | [] -> None
    | (x,y)::zs -> if p x then Some y
                   else findpred p zs


(*find maximum of a list use utilizing some (non-empty list) and none (empty list)*)
let maximum list =
    let rec max lst mx = match lst with
        | []-> mx (* end return cur max *)
        | x::xs -> max xs (if mx < x then x else mx) (* recurse with rest of list as list, and we choose here if we update max or not *)
    in match list with (* calling portion, if we have a blank list, return none otherwise return some - max (some is... sugar?) *)
        | [] -> None
        | x::xs -> Some (max xs x)

(*similar to max above, but max is a placeholder that could take any bool *)
let max gt list = match list with
    | [] -> None
    | x::xs -> let rec max' x xs = match xs with
                   | [] -> x
                   | y::ys -> max' (if gt x y then x else y) ys
               in Some (max' x xs)

(*linear search no higher order *)
let rec find equ? key list = match list with
    | [] -> None
    | (k,v)::xs -> if equ? key k then Some v (* separate the key/value pair from rest of list *)
                   else find equ? key xs (* if not matched above, then recurse with xs *)

(*max of a list find max findmax - using fold left foldl *)
let maxfl gt list = match list with
    | [] -> None
    | x::xs -> Some (List.fold_left
                     (fun n m -> if gt n m then n else m)
                     x xs);;

(*contains - determines if a given element is in the given list *)
let rec contains x list = match list with
    | [] -> false
    | car::cdr -> x = car || contains x cdr;;

(* list reverse *)
let reverse list =
    let rec rev lst out = match lst with
        | [] -> out (* return current reversed portion *)
        | x::xs -> rev xs (x::out) (* recurse with rest of list as new list, elem added to reverse portion *)
    in rev list []

(* trim - takes leading zeros off a list of ints *)
let rec trim list = match list with
    | [] -> []
    | 0::xs -> trim xs
    | list -> list

(* sub subtracts numbers in a list from the value in another list *)
let sub' n1 n2 =
    let rec sub'' n1 n2 borrow = match n1, n2, borrow with
        | n1, [], 0 -> n1
        | n1, [], borrow -> sub'' n1 [borrow] 0
        | h1::t1, h2::t2, borrow ->
          let diff = h1 - h2 - borrow
          in  if diff < 0 then diff + 10 :: sub'' t1 t2 1
                          else diff :: sub'' t1 t2 0
        | _, _, _ -> failwith "sub'"
     in trim (sub'' n1 n2 0)

(*fibonacci *)
let fib n = 
    let rec fib' n a b =
        if n = 0 then a
                 else fib' (n - 1) b (a + b)
    in fib' n 0 1;;

(* merges two lists into one, also orders the result. similar to zip? *)
let rec merge less ls1 ls2 = match ls1, ls2 with
    | [], ls2 -> ls2
    | ls1, [] -> ls1
    | x::xs, y::ys -> 
      if less x y then x :: merge less xs ls2
                  else y :: merge less ls1 ys

(*zip variation -  also takes a function that does the zipping*)
let rec zipwith fn list1 list2 = match list1, list2 with
    | [], [] -> []
    | _, [] -> failwith "zipwith"
    | [], _ -> failwith "zipwith"
    | x::xs, y::ys -> fn x y :: zipwith fn xs ys

(* zip combine lists every other, so [1;2;3] zipped with [a;b;c] = [(1,a);(2,b);(3,c)]*)
let rec zip list1 list2 = match list1, list2 with
    | [], _ -> [] (* if either list is empty, return empty *)
    | _, [] -> [] (* if either list is empty, return empty *)
    | x::xs, y::ys -> (x,y) :: zip xs ys (* return the first from each list, and recurse on the remainder of each list *)

(*unzip from review session *)
let unzip (xs : ('a * 'b) list): 'a list * 'b list = match xs with
    | [] -> ([], [])
    | (fst, snd)::t -> match unzip t with
        | (fstlst, sndlist) -> (fst::fstlist, snd::sndlist)

(*unzip from old solutions *)
let rec unzip list = match list with
    | [] -> ([],[])
    | (a,b)::rest -> let (al,bl) = unzip rest
                     in (a::al, b::bl);;

(*every other every-other  IM NOT SURE HOW IT WORKS*)
let rec every_other list = match list with
    | [] -> [] (* empty list, return empty *)
    | [_] -> list (*  *)
    | x::_::xs -> x::every_other xs;;

(*map from review session *)
let map (f: 'a -> 'b) (xs : 'a list): 'b list = match xs with
    | [] -> []
    | h::t -> (f h)::map f t

(*filter from review session *)
let rec filter (f : 'a -> bool) (xs : 'a list): 'a list = match xs with
    | [] -> []
    | h::t -> if (f h) then h::filter f t else filter f t

(*filter from old solutions *)
let rec filter f list = match list with
    | [] -> []
    | x::xs when f x -> x :: filter f xs
    | _::xs -> filter f xs

let even x = x mod 2 = 0  (* determine if the input number is even or odd *)
List.map even [1;2;3;4] (*list version, returns a matching list of true/false for each elem of given list *)

(* inner product takes two lists of floats, does pairwise multiplication of each element in the lists
    and sums the elements of the resulting list. Uses list.fold_left and list.map2 *)
let ip list1 list2 = (* fold_left takes: operator identity-element list, where list = map operator list1 list2 *)
    List.fold_left (+.) 0. (List.map2 ( *.) list1 list2)

let rec fold_left (f : 'a -> 'b -> 'a) (acc : 'a) (xs : 'b list): 'a = match xs with
    | [] -> acc
    | h::t -> fold_left f (f acc h) t

(* remove duplicates deduplicate  THIS RETURNS BACKWARDS LIST*)
let rec dedup list unique = match list with
  | [] -> unique (* empty list, return empty *) 
  | x::xs -> if (List.memq x unique) then dedup xs unique (*List.memq finds if the given value exists in the list *)
      else dedup xs (x::unique);; (*if value isnt in unqiue already, add it and recurse *)

let rec dedup pred list = match list with
    | [] -> list
    | x::y::xs -> if pred x y then y :: dedup pred xs else x :: y :: dedup pred xs

let rec insert pred item list = match list with
    | [] -> item
    | x::xs -> if pred x item then x :: insert pred item xs else item :: x :: xs

let rec adjacent fn list = match list with
    | x::y::xs -> if fn x y then true else adjacent fn y::xs
    | _ -> false

(*ASNWER KEY ADJACENT*)
let rec adjacent eqv list = match list with
    | [] -> false
    | [_] -> false
    | x::y::_ when eqv x y -> true
    | _::xs -> adjacent eqv xs;;

(* list.hd checks if the first (?) element is the head of a list? -: 'a list -> 'a = <fun> *)
let hd list = match list with
    | [] -> raise (Failure "hd")
    | x::_ -> x (*returns just the head *)

(* list.tl check is first(?) element of the list is the tail? -: 'a list -> 'a list = <fun> *)
let tl list = match list with
    | [] -> raise (Failure "tl")
    | _::xs -> xs (*returns the rest of the list, akin to a cdr *)

(* Binary Search Tree *)
type 'k 'v tree = Tree of (('k, 'v) * ('k 'v tree) option * ('k 'v tree) option)

let key (t: 'k 'v tree): k = match t with
    Tree (k,_,_,_) -> k

let value (t: 'k 'v tree): v = match t with
    Tree (_,v,_,_) -> v

let left (t: 'k 'v tree): t = match t with
    Tree (_,_,l,_) -> l
    
let right (t: 'k 'v tree): t = match t with
    Tree (_,_,_,r) -> r

let binsearch ((less: 'k -> 'k -> bool) (wantkey: 'k) (t: 'k 'v tree)option) :
    'v option = match t with
        | None -> None
        | Some tree when less wantkey (key tree) -> binsearch less wantkey (left tree)
        | Some tree when less (key tree) wantkey -> binsearch less wantkey (right tree)
        | Some tree -> Some (value tree)

(*Find parti even or odd of the number and assign a string*)
let parity = if n mod 2 = 0 then "even" else "odd";;

(*Answer key version of remove duplicates dedup*)
let rec removedups equiv list = match list with
    | [] -> []
    | [_] -> list
    | x::y::zs when equiv x y -> removedups equiv (y::zs)
    | x::y::zs -> x:: removedups equiv (y::zs);;