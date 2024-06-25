loves(a, c).
loves(b, c).
loves(x, y).
loves(y, x).

jealous(X, Y) :- loves(X, Z), loves(Y, Z).
