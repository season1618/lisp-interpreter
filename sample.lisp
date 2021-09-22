(quote 1)
(quote (1 2 3 4))
(atom (quote 1))
(atom (cons (quote 1) (quote 1)))
(eq (cons (quote 2) (quote 3)) (cons (quote 2) (quote 3)))
(car (cons (cons (quote 1) (quote 2)) (quote 3)))
(cdr (cons (cons (quote 1) (quote 2)) (quote 3)))
(cond
    ((eq (quote 1) (quote 1)) (quote 2))
    ((quote t) (quote 3))
)

(define inc (lambda (x) (cons (quote 1) x)))
(inc (quote (1 1)))

(define append
    (lambda (x y)
        (cond
            ((eq x nil) y)
            (t (cons (car x) (append (cdr x) y)))
        )
    )
)
(append (quote (1 2 3)) (quote (4 5 6 7)))

(define f (lambda (x) (cons x (quote 2))))
(define g (lambda (f x) (f x)))
(g f (quote 3))