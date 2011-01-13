console.log("I'm feeling noble!");
console.log("args: " + system.argsn);
console.log("HOME = " + system.env.HOME);

function onKeypress(char) {
  if (char == 113) { // 'q'
    exit();
  } else {
    console.log("Keypress: " + char);
  }
}
