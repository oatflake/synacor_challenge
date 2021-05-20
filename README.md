# Synacor Challenge Solution

https://challenge.synacor.com/

<pre>
The VM has been implemented in C++, see the synacor project.
A savepoint has been created by logging and replaying the input.

Nested Puzzles:
-Coins:
--The permutation has been found using Prolog's constraint solver: coins.pl

-Teleporter:
--Translation of Binary to Assembly has been implemented in C++, see the BinaryToAssembly project.
--The result of BinaryToAssembly can be found in translated_assembly.txt
--teleporter.txt contains some explanations, translating the relevant assembly code (ackermann function) to python.
--A dynamic programming version of the ackermann function has then again been implemented in C++
--Finally the VM has been patched to use the result.

-Orb Maze:
--The path through the maze has been found using iterative deepening implemented in Prolog: orb.pl

-Mirror:
--The string has been reversed and letters have been replaced using Python: mirror.py
</pre>
