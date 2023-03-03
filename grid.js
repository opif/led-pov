// Input grid represented as 2D array of squares
const grid = [
  [0, 1, 1, 1, 0, 1, 1, 1, 0, 0],
  [1, 1, 1, 1, 1, 1, 1, 1, 1, 0],
  [1, 1, 1, 1, 1, 1, 1, 1, 1, 0],
  [1, 1, 1, 1, 1, 1, 1, 1, 1, 0],
  [0, 1, 1, 1, 1, 1, 1, 1, 0, 0],
  [0, 0, 1, 1, 1, 1, 1, 0, 0, 0],
  [0, 0, 0, 0, 1, 0, 0, 0, 0, 0],
];

// □ ■ ■ ■ □ ■ ■ ■ □ □
// ■ ■ ■ ■ ■ ■ ■ ■ ■ □
// ■ ■ ■ ■ ■ ■ ■ ■ ■ □
// ■ ■ ■ ■ ■ ■ ■ ■ ■ □
// □ ■ ■ ■ ■ ■ ■ ■ □ □
// □ □ ■ ■ ■ ■ ■ □ □ □
// □ □ □ □ ■ □ □ □ □ □

// Calculate binary numbers from the columns of the grid
const binaryNumbers = grid[0].map((_, colIndex) => {
  // Get the binary representation of the column
  const binary = grid
    .map((row) => row[colIndex])
    // .reverse()
    .join("");
  // Parse the binary string as a base-2 number
  return parseInt(binary, 2);
});

// Print the binary numbers
console.log(binaryNumbers);
