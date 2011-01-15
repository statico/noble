// Exported variables: exit, system, fs, term

var console = {};

var log = console.log = function() {
  for (var i = 0; i < arguments.length; i++) {
    term.puts(arguments[i] + ' ');
  }
  term.puts('\n');
};

term.clear();
try {
  term.setAttribute('blink');
} catch(e) { term.log(e) }
term.setColor('green', 'blue');
log(" - I'm feeling noble! - ");
term.setColor('default', 'default');
log('args:', system.args);
log('HOME =', system.env.HOME);
log('eval:', eval('1 + 2'));
log('term is', term.width(), 'x', term.height());
log('narrow char - [¢] - wide char - [▣]');

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
