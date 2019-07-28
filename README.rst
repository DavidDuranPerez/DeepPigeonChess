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

To create an executable of the chess engine::

	cd src
	g++ -g main.cpp uci.cpp board.cpp engine.cpp -std=c++11 -o ../dist/DeepPigeon.exe

You can then run the program.

UCI implemented commands
-------------------------

This chess engine uses the Universal Chess Interface protocol for communication. The implemented commands are:

- GUI to engine

  * quit
  * uci
  * position fen [fen_str] moves [moves_str]
  * position startpos moves [moves_str]

- Engine to GUI

  * id name
  * id author
  * uciok

Testing
----------

TBD

Issue Tracker and Mailing List
--------------------------------

Bug reports are welcome!  You can use the `issue tracker <https://github.com/DavidDuranPerez/DeepPigeonChess/issues>`_ to report bugs, and/or submit pull requests `on GitHub <https://github.com/DavidDuranPerez/DeepPigeonChess/pulls>`_.