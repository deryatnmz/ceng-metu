% ?- len([a,b,c,d,e,[a,b],g],X).
% X = 7.

len([], 0).
len([_|T], N) :- len(T, Z), N is Z+1.
