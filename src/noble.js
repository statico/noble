console.log("I'm feeling noble!");

function onKeypress(char) {
  if (char == 113) { // 'q'
    exit();
  } else {
    console.log("Keypress: " + char);
  }
}
