// Exported variables: exit, console, system, fs
// TODO: console should be different from term

console.clear();
console.setColor('green', 'blue');
console.log(" - I'm feeling noble! - ");
console.setColor('default', 'default');
console.log('args:', system.args);
console.log('HOME =', system.env.HOME);
console.log('eval:', eval('1 + 2'));

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
    console.clear()
    break;
  case 'q'.charCodeAt():
    exit()
    break;
  case 'r'.charCodeAt():
    require('noble');
    console.log('reloaded!');
    break;
  default:
    console.log("Keypress: " + char);
  }
}
