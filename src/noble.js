var events = {};
events.onKeypress = function(arguments) {
    puts(' ' + arguments.toString());
};

puts("I'm feeling noble!\n");

function handleKeypress(char) {
  if (char == 113) { // 'q'
    exit();
  } else {
    puts("Keypress: " + char + "\n");
  }
}
