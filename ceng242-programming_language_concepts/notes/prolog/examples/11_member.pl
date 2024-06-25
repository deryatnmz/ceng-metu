% ?- member(a, [a, b, c, d]).
% true .

% ?- member(b, [a, b, c, d]).
% true .

% ?- member(X, [a, b, c, d]).
% X = a ;
% X = b ;
% X = c ;
% X = d ;
% false.

member(X, [X|_]).
member(X, [_|T]) :- member(X, T).
