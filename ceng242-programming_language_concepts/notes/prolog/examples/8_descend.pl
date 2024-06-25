child(martha,charlotte).
child(charlotte,caroline).
child(caroline,laura).
child(laura,rose).

% ?- descend(martha,laura).
% true.
% ?- descend(martha, X).
% X = charlotte ;
% X = caroline ;
% X = laura ;
% X = rose ;
% false.

descend(X, Y) :- child(X, Y).
descend(X, Y) :- descend(Z, Y), child(X, Z).
