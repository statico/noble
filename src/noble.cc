// Copyright 2010 the Noble project authors. All rights reserved.

#include <curses.h>
#include <v8.h>

#include <string>

using namespace std;
using namespace v8;

int main(int argc, char* argv[]) {
  initscr();
  noecho();

  addstr("i'm feeling noble!\n");

  getch();

  endwin();
  return 0;
}
