var instructions = process.argv[2];
var horizontal = 0;
var depth = 0;

const FORWARD = "forward";
const UP = "up";
const DOWN = "down";

instructions = instructions.split("\n");

instructions.forEach(element => {
    let instruction = element.split(" ");
    let value = parseInt(instruction[1])
    if(instruction[0] === FORWARD) {
        horizontal += value;
    } else if (instruction[0] === UP) {
        depth -= value;
    } else if (instruction[0] === DOWN) {
        depth += value;
    }
});

console.log(horizontal * depth);