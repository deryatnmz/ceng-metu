:- module(main, [bigram/2, num_hobbies/2, sentence_match/2]).
:- [kb].

% DO NOT CHANGE THE UPPER CONTENT, WRITE YOUR CODE AFTER THIS LINE


bigram(W, R) :- atom_chars(W, CL), bigrams(CL, R).
bigrams([A, B|_], BG) :- atom_chars(BG, [A, B]).
bigrams([_, B|T], BG) :- bigrams([B|T], BG).


get_hobbies([], []).
get_hobbies([H1|T1], [H2|T2]) :- person(H1, _, H2), get_hobbies(T1, T2).

count_elem([], _, 0).
count_elem([H|T], X, Res) :-
    X=H,
    count_elem(T, X, Z),
    Res is Z+1.
count_elem([H|T], X, Res) :-
    X\=H,
    count_elem(T, X, Res).
    
remove_elem([], _, []).
remove_elem([H|T], X, Res) :-
    H=X,
    remove_elem(T, X, Res).
remove_elem([H|T], X, Res) :-
    H\=X,
    remove_elem(T, X, Z),
    Res = [H|Z].

create_count_list([], []).
create_count_list([H|T], Res) :-
    count_elem([H|T], H, N),
    remove_elem(T, H, T2),
    create_count_list(T2, Z),
    Res = [hobby(H, N)|Z].

num_hobbies(NameList, Res) :-
    get_hobbies(NameList, HobbyList),
    create_count_list(HobbyList, Res).


word_match_acc([], [], Acc, Acc).
word_match_acc([H1|T1], [H2|T2], Acc, Mapping) :- member(map(H1, H2), Acc), word_match_acc(T1, T2, Acc, Mapping).
word_match_acc([H1|T1], [H2|T2], Acc, Mapping) :-
    \+(member(map(H1, _), Acc)),
    \+(member(map(_, H2), Acc)),
    word_match_acc(T1, T2, [map(H1, H2)|Acc], Mapping).

word_match(H1, H2, Acc, Mapping) :-
    atom_chars(H1, W1),
    word(H2),
    atom_chars(H2, W2),
    word_match_acc(W1, W2, Acc, Mapping).

sentence_match([], [], _).
sentence_match([H1|T1], [H2|T2], Mapping) :-
    word_match(H1, H2, Mapping, NewMapping),
    sentence_match(T1, T2, NewMapping).

sentence_match(S1, S2) :- sentence_match(S1, S2, []).
