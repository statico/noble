# Noble: the noble text editor #

I want Noble to be the text editor of the future. I want:

* The speed and flexibility of editing text like Vim
* The external-process integration level of Emacs
* The hot features and OS-level integration of TextMate (but for Linux
  and Windows as well)
* The customizability of writing a whole editor in a functional,
  dynamic language (like Emacs and Elisp)
* The fun and excitement of working with a "hot" language, JavaScript

### Plan ###

1. A basic UI in console
   * V8 + tiny kernel
   * Vim and Emacs emulation
   * Basic modes, selection, undo
1. Ability to hack Noble with Noble
   * Write and save reliably
   * Syntax highlighting
   * Indenting
   * Buffers
1. Get people interested
   * Adopt GitHub
   * Create a central package repo
   * Create a snippets repo
   * Create a feature comparison grid
1. OS Integration
   * Things like drawers, toolbars, built-in help and Growl support on Mac OS X
   * GTK/Gnome support on Linux
   * Fullscreen
1. Add "modern" text editor features
   * Multilingual/RTL ease-of-use
   * IntelliSense
   * Tab/buffer browser
   * Project browser
   * Quickly find functions, methods, files, whatever
   * Snippets
   * Image and SVG support in buffers
   * Great built-in documentation
   * Compatibility modes for Vim, Emacs and Vim (help them wean off)
   * Great color schemes (steal the popular ones)
   * Customization GUI
   * Search and replaces
   * VCS integration
   * As much as possible from
     [Code Bubbles](http://WWW.cs.brown.edu/people/acb/codebubbles_site.HM)
   * Anything else to improve...
     * Latex build/refresh cycle
     * Django refresh cycle

## Building ##

Build and install ncurses, and make sure that the `ncurses5-config` executable is in your path.

Build the v8 library:

    $ python tools/scons-local/scons.py mode=debug

(Note: I needed the extra `arch=x64` parameter on Mac OS X 10.6.)

Then build the noble executable into `src/noble`:

    $ python tools/scons-local/scons.py src

## Hacking ##

### Grand decisions ###

* Simple, simple, simple.
* Write as little code as possible. Write high-level things first and
  then write the smaller functions needed for them to work.
* Steal a lot from Node.js.
* Design things a lot like Emacs and Vim designed things. (They got a
  lot of stuff right.)
* Start with basic, generic implementations. Redesign later.
* Write minimal kernel in C. Optimize later by porting JavaScript to
  C.
* Make packages integration easy to work with. Vim and Emacs are still
  hack-sh.
* Plan for some kind of testing system.

### Layers of Noble ###

Noble should have smart, well-planned out layers. Here's what they are
in order from lowest-level (C) to highest-level (user interface):

1. A small kernel with minimal functions written in C which are made
   available to JavaScript, such as `eval`, `loadFile` and `reallyExit`.
1. An event loop which fires events on keystrokes, system signals, and
   a tick every 1 second.
1. A basic display system (main area, status bar, command line/minibuffer)
1. Buffer management
1. Buffer-rendering system
1. GUI adaptation

### Places where serious help will be needed ###

I have a good idea of what's _possible_ but I don't have the
immediate knowledge of how to _do_ most of it.

Here's a list of things where I can write an initial alpha-level
implementation but will need a better long-term implementation
(preferably written by an expert):

* C++ (compiling, cross-platform support, and -- oh -- the language)
* Build system (v8 uses Scons, I might as well try)
* File IO
* Efficient memory management
* Language parsing
* The rendering layer
* OS integration on Linux, Mac OS X and maybe Windows
* Figuring out how to bundle the core runtime

## Credit ##

* v8
* Node
* TODO: List all important links here

### Contributors ###

* Ian
* Russell

### Stuff ripped off from Node.js ##

* TODO
