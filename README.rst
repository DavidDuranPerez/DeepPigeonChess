This is Deep Pigeon, a chess engine written in C++
======================================================

.. image:: https://travis-ci.org/DavidDuranPerez/DeepPigeonChess.svg?branch=master
   :alt: DeepPigeon build status on Travis CI
   :target: https://travis-ci.org/DavidDuranPerez/DeepPigeonChess

.. image:: https://ci.appveyor.com/api/projects/status/github/DavidDuranPerez/deeppigeonchess?branch=master&svg=true
   :alt: DeepPigeon build status on Appveyor
   :target: https://ci.appveyor.com/project/DavidDuranPerez/deeppigeonchess/branch/master

Copyright (C) 2019, David Duran

.. contents::

Build Instructions
-------------------

To easily create an executable of the chess engine::

	cd src
	g++ -g main.cpp uci.cpp board.cpp utilities.cpp engine.cpp mover.cpp evaluation.cpp -std=c++11 -pthread -o ../dist/DeepPigeon.exe

For a better performance, use O3 optimization (slower to build)::

  cd src
  g++ -g main.cpp uci.cpp board.cpp utilities.cpp engine.cpp mover.cpp evaluation.cpp -std=c++11 -pthread -O3 -o ../dist/DeepPigeon_release.exe

Continuous Integration
-----------------------

This project follows CI for both Linux (Travis) and Windows (AppVeyor), according to the C++11 standard:

+----------+-----------------------------------+
| CI       |     Systems/Compilers             |
+==========+===================================+
| Travis   | Linux (gcc5)                      |
+----------+-----------------------------------+
| AppVeyor | Windows x64 (Visual Studio 2017)  |
+----------+-----------------------------------+


UCI implemented commands
-------------------------

This chess engine uses the Universal Chess Interface protocol for communication. The implemented commands are:

- GUI to engine

  * quit
  * uci
  * isready
  * ucinewgame
  * position 

    - position fen [fen_str] moves [moves_str]
    - position startpos moves [moves_str]
  * go

    - go infinite
    - go depth [depth_int]
  * stop

- Engine to GUI

  * id
    - id name
    - id author
  * uciok
  * readyok
  * bestmove [move_str] ponder [move_str]

Testing
----------

TBD

Issue Tracker and Mailing List
--------------------------------

Bug reports are welcome!  You can use the `issue tracker <https://github.com/DavidDuranPerez/DeepPigeonChess/issues>`_ to report bugs, and/or submit pull requests `on GitHub <https://github.com/DavidDuranPerez/DeepPigeonChess/pulls>`_.