// Copyright 2011 the Noble project authors. All rights reserved.

#ifndef NOBLE_TERM_H_
#define NOBLE_TERM_H_

#include <string>

#include "noble.h"

namespace noble {
namespace term {

using namespace std;
using namespace v8;

Handle<Value> PutCharacter(const Arguments& args);
Handle<Value> MoveCursor(const Arguments& args);
Handle<Value> Clear(const Arguments& args);
Handle<Value> Update(const Arguments& args);
Handle<Value> SetColor(const Arguments& args);
Handle<Value> SetAttribute(const Arguments& args);
Handle<Value> GetWidth(const Arguments& args);
Handle<Value> GetHeight(const Arguments& args);
Handle<Value> GetCursorRow(const Arguments& args);
Handle<Value> GetCursorColumn(const Arguments& args);

void PrintString(const string& message);
void PrintLine(const string& message);
void ReportException(const string& prefix, const TryCatch& try_catch);
void PauseAndDisplayMessage(const string& message);
int WaitForKeypress();
void MainLoop();
void Finish();

Handle<Value> Initialize();

} } // namespace noble::term

#endif // NOBLE_TERM_H_
