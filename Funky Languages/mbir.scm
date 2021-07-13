#!/afs/cats.ucsc.edu/courses/cse112-wm/usr/racket/bin/mzscheme -qr
;; $Id: 1498579 mbir.scm,v 1.9 2021-01-12 11:57:59-08 - - $
;;
;; NAME
;;    mbir.scm filename.mbir
;;
;; AUTHOR
;;  All code written by Kyle Serio. All professor 
;;  provided code has been removed.
;; SYNOPSIS
;;    mbir.scm - mini basic interper
;;
;; DESCRIPTION
;;    The file mentioned in argv[1] is read and assumed to be an mbir
;;    program, which is the executed.  Currently it is only printed.
;;


;;Full function list, including those provided 
;;that have been removed. For Reference.
(define *DEBUG* #f)
(define *STDIN* (current-input-port))
(define *STDOUT* (current-output-port))
(define *STDERR* (current-error-port))
(define *ARG-LIST* (vector->list (current-command-line-arguments)))

(define *stmt-table*     (make-hash))
(define *function-table* (make-hash))
(define *var-table*      (make-hash))
(define *array-table*    (make-hash))
(define *label-table*    (make-hash))

;;Defines a set of functions to interpret.
(for-each
    (lambda (symfun) (hash-set! *function-table* 
                     (car symfun) (cadr symfun)))
    `(
        (+    ,+)
        (-    ,-)
        (*    ,*)
        (/    ,/)
        (=    ,=)
        (>    ,>)
        (<    ,<)
        (>=   ,>=)
        (<=   ,<=)
        (!=   ,(lambda (x y) (not (equal? x y))))
        (^    ,expt)
        (sqrt ,sqrt)
        (sqr  ,sqr)
        (exp  ,exp)
        (log  ,log)
        (sin  ,sin)
        (cos  ,cos)
        (tan  ,tan)
        (asin ,asin)
        (acos ,acos)
        (atan ,atan)
        (abs  ,abs)
        (ceil , ceiling)
        (floor ,floor)
        (round ,round)
    ))

(define NAN (/ 0.0 0.0))

;;Central function that dissects a expression,
;;allowing individual portions to be reduced.
;;Handles numbers, symbols (from table), arrays,
;;mathematic expressions with unlimited depth.
(define (eval-expr expr)
    (cond ((number? expr) (+ expr 0.0))
          ((symbol? expr) (hash-ref *var-table* expr 0.0))
          ((pair? expr)
            (if (equal? (car expr) 'asub ) ;;cadr = array, caddr = elem
                (vector-ref (hash-ref *array-table* (cadr expr) #f) 
                            (exact-round (eval-expr (caddr expr))))
                (let ((func (hash-ref *function-table*(car expr) #f))
                    (opnds (map eval-expr (cdr expr))))
                    (if (not func) NAN 
                        (apply func opnds)))))
          (else NAN)))

;;Handles array creation
(define (interp-dim args continuation)
    (let ((dim (eval-expr (caddar args))))
    (when (equal? (caar args) 'asub )
            (hash-set! *array-table* (cadar args) 
                       (make-vector (exact-round dim) 0.0))
            (hash-set! *var-table* (cadar args) (exact-round dim))))
    (interp-program continuation))

;;handles array access/modification and subscripting
(define (interp-vec args) 
    (let ((size (hash-ref *var-table* (cadar args) 0.0)))
    (cond ((< (exact-round (eval-expr (caddar args))) 0)
              (printf "Can't Use subscript < 0!~n"))
          ((>= (exact-round (eval-expr (caddar args))) size) 
                    (printf "Can't Use Subscript >= Size!~n"))
          (else (vector-set! (hash-ref *array-table* (cadar args) #f) 
                              (exact-round (eval-expr (caddar args))) 
                              (eval-expr (cadr args)))))))

;;Handles variable assignment
(define (interp-let args continuation)
    (let ((right (eval-expr (cadr args))))
         (cond ((symbol? (car args)) 
                (hash-set! *var-table* (car args) right))
               ((pair? (car args)) 
                   (if (equal? (caar args) 'asub ) 
                       (interp-vec args)
                       (printf "pair, but not asub~n")))
               (else (printf "not pair or symbol!~n"))))
    (interp-program continuation))

;;Handles program execution ordering/control
(define (interp-goto args continuation)
    (let ((label (hash-ref *label-table* (car args) #f)))
        (if (equal? label false)  
            (printf "Label Undefined!~n")   
            (interp-program label))))

;;Handles if expressions
(define (interp-if args continuation)
    (if (equal? (eval-expr (car args)) #t)
        (interp-goto (list(cadr args)) continuation)
        (interp-program continuation)))

;;Handles the reading of numeric input
(define (readnum)
    (let ((object (read))
        (nan (/ 0.0 0.0)))
        (when (eof-object? object) (hash-set! *var-table* 'eof 1))
        (cond ((number? object) (+ object 0.0))
              (else nan))))

;Handles user input
(define (interp-input args continuation)
    (let ((num (readnum)))
         (if (equal? (hash-ref *var-table* 'eof ) 1)
             (interp-program continuation)
             (begin (hash-set! *var-table* (car args) num)
                    (if (null? (cdr args))
                        (interp-program continuation)
                        (interp-input (cdr args) continuation))))))

;;Handles the scanning of the given code, and pulls the
;;labels and their associated continuation
(define (scan-for-labels program)
        (when (not (null? program))
            (let ((label (line-label (car program))))
                (when (symbol? label)
                        (hash-set! *label-table* label program)))
            (scan-for-labels (cdr program))))
