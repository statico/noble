Noble: the noble text editor
============================

Building
--------

This works at the moment:

    $ python tools/scons-local/scons.py mode=debug
    # python tools/scons-local/scons.py src

Goals
-----

* The speed and flexibility of editing text like Vim
* The external-process integration level of Emacs
* The hot features and OS-level integration of TextMate
* The expressibility, customizablity and hackability of being built mostly in a popular, modern language

Objectives
----------

1. A basic UI in console
   * V8 + tiny kernel
   * Vim and Emacs emulation
   * Basic modes, selection, undo
1. Ability to hack Noble with Noble
   * Write and save reliably
   * Syntax highlighting
   * Indenting
   * Buffers
1. OS Integration
   * Things like drawers, toolbars, built-in help and Growl support on Mac OS X
   * GTK/Gnome support on Linux
1. Add "modern" text editor features
   * IntelliSense
   * Project browser
   * Quickly find functions, methods, files, whatever
   * Snippets
   * Image and SVG support in buffers

Decisions
---------

* Simple, simple, simple.
* Steal a lot from Node.js. (We want GUI, not Console.)
* Design things a lot like Emacs and Vim designed things. (They got a lot of stuff right.)
* Start with basic, generic implementations. Redesign later.
* Write minimal kernel in C. Optimize later by porting JavaScript ot C.
* Make packages integration easy to work with. Vim and Emacs are still hack-ish.

Building
--------

