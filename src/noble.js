// Exported variables: exit, system, fs, term

var console = {};

var log = console.log = function console_log() {
  // Draws a character to the string, wrapping to the next line if
  // necessary.
  function put_char(char) {
    var cw = term.characterWidth(char);
    if (cw == 0) return;

    var gap = term.width() - term.x();
    if (cw > gap) term.putCharacter('\n');

    // Draw the character. If it's a newline, SLang does the right
    // thing automatically. If it's wide it'll need to be padded with
    // spaces to the right of it.
    term.putCharacter(char);
    if (cw > 1) {
      for (var i = 1; i < cw; i++) {
        term.putCharacter(' ');
      }
    }
  }

  // `arguments' isn't a normal array, unforunately.
  for (var i = 0; i < arguments.length; i++) {
    var arg = arguments[i].toString();
    for (var j = 0; j < arg.length; j++) {
      put_char(arg[j]);
    }
    put_char(i == arguments.length - 1 ? '\n' : ' ');
  }

  term.update();
};

function require(name) {
  // A naive implementation of http://wiki.commonjs.org/wiki/Modules/1.1
  var paths = (system.env.NOBLE_PATH || '.').split(/:/);
  var content, object;
  for (var i = 0, path; path = paths[i]; i++) {
    try {
      content = fs.read(path + '/' + name + '.js'); // derp derp
    } catch (e) {
      continue;
    }
    if (content == undefined) {
      continue;
    }
    return eval(content);
  }
  throw new Error("Couldn't open " + name + " in " + paths);
}

var buffers = [], currentBuffer; // TODO: support windows

function redrawDisplay() {
  if (buffers.length < 1) {
    buffers.push({name: 'scratch', content: '你好! Welcome to Noble!'});
  }
  if (currentBuffer == undefined or buffers.indexOf(currentBuffer) == -1) {
    currentBuffer = buffers[0];
  }

  term.clear();
  if (term.height() <= 3) {
    term.moveCursor(0, 0);
    console.log("Terminal is too short.");
    return;
  }

  redrawStatusBar();
  redrawCurrentBuffer();
}

function redrawStatusBar() {
  term.moveCursor(0, term.height() - 2);
  term.setAttribute('reverse');
  var text = ' ' + currentBuffer.name + ' ';
  var x = 0;
  while (x < term.width()) {
    if (x < text.length()) {
      var c = text[x], cw = term.characterWidth(c);
      term.putCharacter(c);
      for (var i = 1; i <= cw; i++) {
        term.putCharacter(' ');
      }
      x += cw;
    } else {
      term.putCharacter(' ');
      x++;
    }
  }
}

function onKeypress(char) {
  switch (char) {
  case 'c'.charCodeAt():
    term.clear()
    break;
  case 'q'.charCodeAt():
    exit()
    break;
  case 'r'.charCodeAt():
    require('noble');
    log('reloaded!');
    break;
  default:
    log("Keypress: " + char);
  }
}
