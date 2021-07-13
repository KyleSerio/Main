;; random info - 
;; cadadadadadadr - evaluate right to left, with a = first elem or sub-list, and d = the rest not in a.
;; ex: (1 2 3) : (1) = car, (2 3) = cdr, 2 = cadr, 3 = cddr

;; IN SOL - they use (if (or (null? list) (null? (cdr list)) list
;;                       (cons (car list) (every-other (cddr list))))
(define (everyother list index) ;;prints out every other element of a list - would the index thing be a nono?
    (if (equal? index 0)
        ((printf "Elem: ~s\n" (car list))
            (if (null? (cdr list))
                (printf "Done!\n")
                (everyother (cdr list) 1)))
        ((printf "Skip!\n")
            (if (null? (cdr list))
                (printf "Done!\n")
                (everyother (cdr list) 0)))
    )
)

;;merge - uses a function to do the zipping
(define (merge less ls1 ls2)
    (cond ((null? ls1) ls2)
          ((null? ls2) ls1)
          ((less (car ls1) (car ls2))
                 (cons (car ls1) (merge less (cdr ls1) ls2)))
          (else (cons (car ls2) (merge less ls1 (cdr ls2))))))


;; finds the gcd - greatest common denominator of two given numbers
(define (gcd x y)
    (cond ((> x y) (gcd (- x y) y))
          ((< x y) (gcd x (- y x)))
          (else x)))

;;length - finds the length of a given list
(define (length list) (foldl (lambda (_ x) (+ x 1)) 0 list))

;;length - finds lengths of given list, no higher order
(define (length list)
    (define (len lst acc)
        (if (null? lst) acc
            (len (cdr lst) (+ acc 1))))
    (len list 0))

;;sum the elements of two lists, creating a third list ex: (1 2) + (2 3) = (3 5)
(define (add num1 num2)
    (define (addc num1 num2 carry)
        (cond ((and (null? num1) (= carry 0)) num2)
              ((and (null? num2) (= carry 0)) num1)
              ((null? num1) (addc (list carry) num2 0))
              ((null? num2) (addc num1 (list carry) 0))
              (else (let ((sum (+ (car num1) (car num2) carry)))
                         (cons (remainder sum 10)
                         (addc (cdr num1) (cdr num2)
                               (floor (/ sum 10))))))))
    (addc num1 num2 0))

(define (sum list) ;; uses fold to sum up elements of a list, takes a list returns a number
    (printf "Fold: ~s\n" (foldl + 0 list))
)
;; solution sum no fold without fold
(define (sum list) 
    (define (summ list n)
        (if (null? list) n ;; end, return running total
            (summ (cdr list) (+ n (car list))))) ;; recurse with rest of list, and running total + car as new total
    (summ list 0)) ;; calls inner

;;solution sum using fold
(define (sum list) (fold_left (lambda (a b) (+ a b)) 0 list))

;;Define fold_left, w/ args: folding function, unit value, list. The unit value is leftmost arg to fold func
(define (fold_left fn unit list)
    (if (null? list) unit ;; end - return the base unit
        (fold_left fn (fn unit (car list)) (cdr list)))) ;; else - fold fn (inner fn call on car) (rest of list)

(define (even x) (= 0 (remainder x 2))) ;;determines if a single number is even
(map even '(1 2 3 4))  ;;determines if a list of numbers are even - returns a list of true/false that match the given elements

(define (even list) ;;uses a map function on each element of a list to replace them with the function's output
    (printf "map: !~s\n" (map (lambda (x) (equal? (remainder x 2) 0)) list))
)

;;define map
(define (map f list)
        (if (null? list) '()
            (cons (f (car list)) (map f (cdr list)))))

(define (maximum list) ;; max maximum from solutions
    (define (max lst mx)
        (cond ((null? lst) mx) ;; end, return max
              ((< mx (car lst)) (max (cdr lst) (car lst)));; if car > max, recurse with car as max
              (else (max (cdr lst) mx)))) ;; otherwise we still have max, so recurse on rest of list w/ max
    (if (null? list) #f ;; first part called in the outer function (?)
        (max (cdr list) (car list)))) ;; start inner call using the first element as max

(define (maximum gt list) ;; SAME as above, but with any operator (max/min/=)
    (define (max big listt)
        (if (null? listt) big ;; end - return big (?)
            (let ((a (car listt)) ;; def a as car - why tho
                  (d (cdr listt))) ;; def d as cdr - why tho
                 (if (gt big a) (max big d) ;; gt is the operator (??) so if big > a, recurse and bring big forward
                                (max a d))))) ;; if ^ isn't true, call max a d, bringing a forward
    (if (null? list) #f 
        (max (car list) (cdr list))))

(define (findMax list max) ;; main function for findMax - uses the max as an arg to keep track
    (if (null? (cdr list))
        (if (> (car list) max)
            (printf "Max: ~s\n" (car list))
            (printf "Max: ~s\n" max))
        (if (> (car list) max)
            (findMax (cdr list) (car list))
            (findMax (cdr list) max)))
)

(define (maximum list) ;; caller func to find max of a list
    (printf "List: ~s\n" list)
    (findMax list 0)
)

;;factorial - return false for a negative number
(define (fac n)
        (define (facc nn mm)
                (if (<= nn 1) mm
                    (facc (- nn 1) (* nn mm))))
        (if (< n 0) #f (facc n 1)))

(define (fibonacci old new) ;; fibonacci self made
    (printf "~s \n" old)
    (if (> new 100)
        (printf "done!\n")
        (fibonacci new (+ old new))))

(define (reverse list) ;;reverse list, from solutions
    (define (rev lst out) ;; nested the caller/inner function
        (if (null? lst) out ;; once we are done, out has the reversed list
            (rev (cdr lst) (cons (car lst) out)))) ;; recurse with the rest of the list and add current car to out, 
                                                   ;; which contains the reversed list so far
    (rev list '()))

;;reverse list, using higher order function
(define (foldl_rev list)
    (foldl (lambda (a d) (cons a d)) '() list))

;; drop/removes the first n elements from the list
(define (drop1 n list)
    (cond ((null? list) '())
          ((<= n 0) list)
          (else (drop1 (- n 1) (cdr list)))))

;;find linear search, using cond
(define (find equ? key list)
    (cond ((null? list) #f) ;; no list return false
          ((equ? key (caar list)) (cadar list)) ;; we found it, key == caar list, return cadar
          (else (find equ? key (cdr list))))) ;; not found, recurse on rest of list

;;Scheme binary tree binary search
;;defines:
(define (key tree) (car tree))
(define (value tree) (cadr tree))
(define (left tree) (caddr tree))
(define (right tree) (cadddr tree))

;;binary search - doesn't (cant) use equal, so does 2x less than: a < b and b < a both false means =
(define (binsearch less wantkey tree) ;; less is a compare func, wantkey is the searched for key, tree is root node
    (cond ((null? tree) #f) ;; indicates we didn't find wantkey
          ((less wantkey (key tree)) ;; fixed
                (binsearch less wantkey (left tree))) ;; recurse on the left tree - key > wantkey
          ((less (key tree) wantkey) ;; compare key tree with wantkey - key tree < wantkey
                (binsearch less wantkey (right tree)));; key tree < wantkey, so go right
          (else (value tree))));; if neither of the above triggers, then this is our key!

(define (evalexpr expr) ;; evalexpr - hard one, this is from solutions
    (if (number? expr) (+ expr 0.0) ;; nested arbitrarily deep. If number, we hit "bottom"
        (apply (hash-ref fnhash (car expr)) ;; (apply?) the stated function to the expression 
                (map evalexpr (cdr expr))))) ;; recursive call on the rest of the expression

;;zip function zips two lists together into a list of pairs
(define (zip ls1 ls2)
        (if (or (null? ls1) (null? ls2)) '()
            (cons (list (car ls1) (car ls2))
                  (zip (cdr ls1) (cdr ls2)))))

;;zipwith function - same as above but uses a separate function to do the zipping
(define (zipwith error fn list1 list2)
    (cond ((null? list1) (if (null? list2) '() error))
          ((null? list2) error)
          (else (let ((tail (zipwith error fn (cdr list1) (cdr list2))))
                     (if (eqv? error tail) error
                         (cons (fn (car list1) (car list2)) tail))))))

;;contains - finds if a given element is in the list
(define (contains x list)
        (if (null? list) #f
            (or (= x (car list)) (contains x (cdr list)))))

;;filter function - takes predicate and list returns a list containing the same ordering as og list, but only elems that 
;;                  match the predicate condition
(define (filter p? list)
        (if (null? list) '()
            (let ((a (car list))
                  (d (cdr list)))
                 (if (p? a) (cons a (filter p? d))
                            (filter p? d)))))

;; predicate - finds the first pair in a list of key/val pairs that match the given predicate, and returns the matching value
(define (findpred none p list)
    (cond ((null? list) none)
          ((p (caar list)) (cadar list))
          (else (findpred none p (cdr list)))))

;;odd length - determines if a list has an odd length
(define (oddlen list)
        (cond ((null? list) #f)
              ((null? (cdr list)) #t)
              (else (oddlen (cddr list)))))


;;find duplicates remove duplicates - THIS WORKS, but issue with parens somewhere---vvvv
;;note- instead of isin, can use this: (equal? false (memq elem list)) IE (equal? false (memq 1 '(2 3 4)))
;;                                    since memq returns false if the elem doesn't exist, or a list if it does
;;                                    so if the above evaluates to true, then the value is not present!
(define (isin val list)
    (cond ((null? list) #f)
          ((equal? val (car list)) #t)                                          ;;
          (else (isin val (cdr list)))))                                        ;;

(define (dedupin list unique)
    (if (null? list) 
        (unique)                                                                ;;
        (if (isin (car list) unique) ;; so - could be if (equal? false (memq (car list) unique))
            (if (null? (cdr list))  ;;      BUT - returns true if val is NOT in unique, so reverse then/else of the if
                 (unique)
                 ((dedupin (cdr list) unique)))                                 ;;
            (if (null? (cdr list))
                 (cons (car list) unique)
                 ((dedupin (cdr list) (cons (car list) unique)))))))            ;;
            
(define (dedup list)
    (let ((unique (dedupin list '())))
         (printf "Original: ~s \nUnique: ~s\n" list unique)))
;; --------------------------------------------------------------------------------^^^^^

(define (adjacent fn list)
    (cond ((null? (cdr list)) #f)
          ((equal? (fn (car list) (cadr list)) #t) #t)
          (else (adjacent fn (cdr list)))))

;;ANSWER KEY ADJACENT
(define (adjacent eqv list)
    (cond ((or (null? list) (null? (cdr list))) #f)
          ((eqv (car list) (cadr list)) #t)
          (else (adjacent eqv (cdr list)))))

(define (dedup pred list)
    (cond ((null? (cdr list)) list)
          ((pred (car list) (cadr list)) (dedup pred (cdr list)))
          (else (cons (car list) (dedup pred (cdr list))))))


(define (insert pred item list)
    (cond ((null? list) list item)
          ((pred (car list) item) (cons value list))
          (else (cons (car list) (insert pred item (cdr list))))))

;; Determine parity of even or odd and assign a string
(define parity (if (= (remainder n 2) 0) "even" "odd"))    

;;answer key removedups remove duplicates
(define (removedups same? list)
    (if (or (null? list) (null? (cdr list))) list
        (let ((head (car list))
              (tail (removedups same? (cdr list))))
             (if (same? head (cadr list))
                 (removedups same? tail)
                 (cons head (removedups same? tail))))))
;;Compose
(define (compose f g)
    (lambda (x) (f (g x))))

;;Filter using compose
(define (filter ok? lst)
    (if (null? lst) '()
        (let ((a (car lst))
              (d (cdr lst)))
             (if (ok? a) (cons a (filter ok? d))
                         (filter ok? d)))))

;;Split using the last two
(define (split ok? lst)
    (let ((ok (filter ok? lst))
          (not-ok (filter (compose not ok?) lst)))
         (cons ok (cons not-ok '()))))
