% ?- a2b([a,a,a],[b,b,b]).
% true.

% ?- a2b(X, [b,b]).
% X = [a, a].

a2b([], []).
a2b([a|T1], [b|T2]) :- a2b(T1, T2).
