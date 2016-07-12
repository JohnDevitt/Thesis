send([[S,E,N,D], [M,O,R,E], [M,O,N,E,Y]]) :-
    Digits = [S,E,N,D,M,O,R,Y],
    Carries = [C1,C2,C3,C4],
    Digits in 0..9,
    Carries in 0..1,
    M #= C4,
    O + 10 * C4 #= M + S + C3,
    N + 10 * C3 #= O + E + C2,
    E + 10 * C2 #= R + N + C1,
    Y + 10 * C1 #= E + D,
    M #>= 1,
    S #>= 1,
    all_different(Digits),
    label(Digits).


schedule(Ts) :-
    Ts = [[1,2,0,1],[2,3,4,5],[2,3,0,1],[3,4,5,6],[3,4,2,3],[3,4,8,9]].
path(A, D, Ps) :-
    schedule(Ts),
    Ps = [[A,B,_T0,T1],[B,C,T2,T3],[C,D,T4,_T5]],
    tuples_in(Ps, Ts),
    T2 #> T1,
    T4 #> T3.

/*
An example query:
?- path(1, 4, Ps), flatten(Ps, Vars), label(Vars).
*/
