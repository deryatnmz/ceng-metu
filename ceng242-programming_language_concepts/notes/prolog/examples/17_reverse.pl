% ?- reverse([1, 2, 3, 4], [4, 3, 2, 1]).
% true.

% ?- reverse([1, 2, 3, 4], X).
% X = [4, 3, 2, 1].


acc_reverse([], Acc, Acc).
acc_reverse([H|T], Acc, Res) :- acc_reverse(T, [H|Acc], Res).

reverse(X, Y) :- acc_reverse(X, [], Y).
