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

term.clear();
try {
  term.setAttribute('blink');
} catch(e) { log(e) }
term.setColor('green', 'blue');
log(" - I'm feeling noble! - ");
term.setColor('default', 'default');
log('args:', system.args);
log('HOME =', system.env.HOME);
log('eval:', eval('1 + 2'));
log('term is', term.width(), 'x', term.height());
log('narrow char - [¢] - wide char - [▣]');
log('a黑z');

var modules = {};

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
