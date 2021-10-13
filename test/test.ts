// don't you dare change those imports. They must be explicit to the index file, otherwise the emitted files are being considers input files and cannot be overwritten. 'Cannot write file "./dist/index.d.ts" because it would overwrite input file'
import Measuresuite from "../index";
import type { MeasureResult } from "../index";
import { zip } from "lodash";
import { functionA, functionB, width, numArgsIn, numArgsOut } from "./functions_sq.test";

const chunkSize = 16;
const numBatches = 100;
const batchSize = 200;
Measuresuite.libcheckfunctionssuffix = "clang-AMD";
try {
  const bounds = Array(width).fill("0x18000000000000");

  var measureTest = new Measuresuite(
    width,
    numArgsIn,
    numArgsOut,
    chunkSize,
    bounds,
    "fiat_curve25519_carry_square",
  );
} catch (e) {
  console.error(e);
  process.exit(1);
}
// MAYBE, not sure on the pt_double

let result: MeasureResult | null = null;
let result2: MeasureResult | null = null;
try {
  console.log("len a " + functionA.length);
  console.log("len b " + functionB.length);
  result = measureTest.measure(functionA, functionB, numBatches, batchSize);
  result2 = measureTest.measure(functionB, functionB, numBatches, batchSize);
} catch (e) {
  console.error("excution of measure failed.");
  console.error(e);
}

if (!result || !result2) {
  console.error(
    "Measure was executed without throwning an error, but the result is nullish. This should not happen. Exiting",
  );
  process.exit(1);
}

if (!result.stats.checkResult || !result2.stats.checkResult) {
  console.error("Wrong result");
  process.exit(1);
}

export function getMedian(arr: Array<number | undefined>): number {
  const cleaned: number[] = arr.filter((v) => typeof v === "number" && v > 0) as number[];
  const sorted = cleaned.sort((a, b) => a - b);
  const len = sorted.length - 1;
  return len % 2
    ? // uneven
      // e.g. c.length=4; len=3; return (c[1]+c[2])/2
      (sorted[(len - 1) / 2] + sorted[(len + 1) / 2]) / 2
    : // e.g. c.length = 5; len=4; return c[2]
      sorted[len / 2];
}

function printres(res: MeasureResult): void {
  let [medianA, medianB, medianCheck] = zip(...res.times).map(getMedian);
  console.log(
    `#Batches: ${numBatches}, BatchSize ${batchSize} median cycles:\nA         | B         | Check     | ratio A / b \n` +
      `${medianA}`.padEnd(10) +
      "| " +
      `${medianB}`.padEnd(10) +
      "| " +
      `${medianCheck}`.padEnd(10) +
      "| " +
      `${(medianA / medianB).toFixed(4)}`.padEnd(10),
  );
  console.log(medianA < medianB ? "A is better than B\n" : "B is better or equal to A\n");
}

console.log("\nresult1: should be very different.");
printres(result);
console.log("\nresult2: should be very similar.");
printres(result2);

console.log("\n\nTested sucessful."); // cause no error was thrown beforehand.
