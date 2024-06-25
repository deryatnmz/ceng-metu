% ?- append([a,b,c], [1,2,3], [a,b,c,1,2,3]).
% true.

% ?- append([a,b,c], [1,2,3], X).
% X = [a, b, c, 1, 2, 3].

% ?- append([a,b,c], X, [a,b,c,1,2,3]).
% X = [1, 2, 3].

% ?- append(X, [1,2,3], [a,b,c,1,2,3]).
% X = [a, b, c] .


append([], List2, List2).
append([H|T1], List2, [H|T3]) :- append(T1, List2, T3).
