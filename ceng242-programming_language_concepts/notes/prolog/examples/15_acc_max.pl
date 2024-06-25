% ?- acc_max([1,0,5,4], 0, X).
% X = 5 ;
% false.


acc_max([], Acc, Acc).
acc_max([H|T], Acc, Res) :-
    H > Acc,
    acc_max(T, H, Res).
acc_max([H|T], Acc, Res) :-
    H =< Acc,
    acc_max(T, Acc, Res).
    
