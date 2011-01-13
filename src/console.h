// Copyright 2011 the Noble project authors. All rights reserved.

#ifndef NOBLE_CONSOLE_H_
#define NOBLE_CONSOLE_H_

#include <string>

#include "noble.h"

namespace noble {
namespace console {

using namespace std;
using namespace v8;

Handle<Value> Log(const Arguments& args);
Handle<Value> MoveCursor(const Arguments& args);
Handle<Value> Clear(const Arguments& args);
Handle<Value> Update(const Arguments& args);
Handle<Value> SetColor(const Arguments& args);
Handle<Value> SetAttribute(const Arguments& args);

void PrintString(const string& message);
void PrintLine(const string& message);
void PrintException(const string& prefix, const TryCatch& try_catch);
void PauseAndDisplayMessage(const string& message);
int WaitForKeypress();
void MainLoop();
void Finish();

Handle<Value> Initialize();

} } // namespace noble::console

#endif // NOBLE_CONSOLE_H_
