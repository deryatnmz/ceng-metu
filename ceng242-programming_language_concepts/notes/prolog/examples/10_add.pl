% ?- add(succ(succ(0)),succ(succ(0)),succ(succ(succ(succ(0))))).
% true.
% ?- add(succ(succ(0)),succ(0),Y).
% Y = succ(succ(succ(0))).

add(0, Y, Y).
add(succ(X), Y, succ(Z)) :- add(X, Y, Z).
