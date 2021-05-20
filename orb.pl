:- use_module(library(clpfd)).

% maze layout:
% 1	-	8	*
% *	11	*	4
% 18	-	4	+
% *	9	-	22
%
% start coord: (3, 3)
% vault coord: (1, 1)
% vault: 30

node((1, 1), 1).
node((3, 1), 8).
node((2, 2), 11).
node((4, 2), 4).
node((1, 3), 18).
node((3, 3), 4).
node((2, 4), 9).
node((4, 4), 22).

op((2, 1), sub).
op((4, 1), mul).
op((1, 2), mul).
op((3, 2), mul).
op((2, 3), sub).
op((4, 3), add).
op((1, 4), mul).
op((3, 4), sub).

add(A, B, Z) :- Z #= A + B.
mul(A, B, Z) :- Z #= A * B.
sub(A, B, Z) :- Z #= A - B.

opCoords(X, N1Y, X, N2Y, X, OY) :- OY #= (N1Y + N2Y) // 2.
opCoords(N1X, Y, N2X, Y, OX, Y) :- OX #= (N1X + N2X) // 2.
opCoords(N1X, N1Y, N2X, N2Y, OX, OY) :-
    N1X #\= N2X,
    N1Y #\= N2Y,
    OX #= N1X,
    OY #= N2Y.
opCoords(N1X, N1Y, N2X, N2Y, OX, OY) :-
    N1X #\= N2X,
    N1Y #\= N2Y,
    OX #= N2X,
    OY #= N1Y.

edge(((N1X, N1Y), N1V), ((N2X, N2Y), N2V), Z, (OX, OY)) :-
    abs(N1X - N2X) + abs(N1Y - N2Y) #= 2,
    Z #> 0,
    N2X * N2Y #\= 16,
    ( N2X * N2Y #= 1 -> Z #= 30 ; true ),
    opCoords(N1X, N1Y, N2X, N2Y, OX, OY),
    op((OX, OY), Op),
    call(Op, N1V, N2V, Z).

path(AC, BC, Orb, [EC, BC], NewOrb) :-
    node(AC, _),
    node(BC, BV),
    edge((AC, Orb), (BC, BV), NewOrb, EC).
path(AC, CC, Orb, [EC, BC|P], NewOrb) :-
    node(AC, _),
    node(BC, BV),
    node(CC, _),
    edge((AC, Orb), (BC, BV), Orb1, EC),
    path(BC, CC, Orb1, P, NewOrb).

ids(L) :- length(L, _), path((4, 4), (1, 1), 22, L, 30), !.

direction(1, 0, west).
direction(-1, 0, east).
direction(0, 1, south).
direction(0, -1, north).

convertPath(_, [], []).
convertPath((SX, SY), [(GX, GY)|T], [D|Ds]) :-
    DX #= GX - SX,
    DY #= GY - SY,
    direction(DX, DY, D),
    convertPath((GX, GY), T, Ds).

solve :- ids(L), convertPath((4, 4), L, R), writeln(R).


