// Copyright 2011 the Noble project authors. All rights reserved.

#ifndef NOBLE_CONSOLE_H_
#define NOBLE_CONSOLE_H_

#include <string>

#include "noble.h"

namespace noble {
namespace console {

using namespace std;
using namespace v8;

void Clear();
void MoveCursor(int x, int y);
void PrintString(const string& message);
void PrintLine(const string& message);
void PrintException(const string& prefix, const TryCatch& try_catch);
void PauseAndDisplayMessage(const string& message);
int WaitForKeypress();
void MainLoop();
void Finish();
void Initialize(Handle<Object> target);

} } // namespace noble::console

#endif // NOBLE_CONSOLE_H_
