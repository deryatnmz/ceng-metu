% ?- acc_len([a,b,c,d,e,[a,b],g], 0, X).
% X = 7.

acc_len([], Acc, Acc).
acc_len([_|T], Acc, Res) :- Z is Acc+1, acc_len(T, Z, Res).
