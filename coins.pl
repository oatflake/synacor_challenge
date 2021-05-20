:- use_module(library(clpfd)).
puzzle([AN,BN,CN,DN,EN]) :-
    permutation([(A,AN),(B,BN),(C,CN),(D,DN),(E,EN)],
                [(2,red),(3,corroded),(5,shiny),(7,concave),(9,blue)]),
    A + B * C * C + D * D * D - E #= 399.
