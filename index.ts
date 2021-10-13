import { existsSync } from "fs";
import ms from "measure-addon";

import type { MeasureResult } from "./measure.interface";
export type { MeasureResult };

// use with caution
export const native_ms = {
  init: ms.measuresuite_init,
  measure: ms.measuresuite_measure,
};

export default class Measuresuite {
  private static _libcheckfunctionsFilename: string;
  public static chunkSize: number;

  // the idea behind that is, to have different libcheckfunctions-p256-mul.so to be able to run multiple instances at the same time. otherwise one would overwrite the other
  public static set libcheckfunctionssuffix(suff: string) {
    this._libcheckfunctionsFilename = `${__dirname}/libcheckfunctions-${suff}.so`;
  }

  public static get libcheckfunctionsFilename(): string {
    if (!Measuresuite._libcheckfunctionsFilename) {
      throw new Error("set Measuresuite.libcheckfunctionssuffix first;");
    }
    return Measuresuite._libcheckfunctionsFilename;
  }

  /**
   *
   * @param bounds is a string array of length @param argwidth. It must contain hex-numbers representing a mask or an upper bound.
   * if the string ends with a '0/2/4/8' it is assumed to be a bound.
   * if the string ends with a '1/3/5/9/f' it is assumed to be a mask.
   * if it is considered a bound, the mask is compuded via subtracting 1.
   * bounds may also be an empty array, in which case -1 as the mask is assumed. (all bits used, no effective bound.)
   */
  public constructor(
    argwidth: number,
    argNumIn: number,
    argNumOut: number,
    chunkSize: number,
    bounds: string[],
    functionSymbol: string,
  ) {
    Measuresuite.chunkSize = chunkSize;
    if (!existsSync(Measuresuite.libcheckfunctionsFilename)) {
      throw new Error(
        `${Measuresuite.libcheckfunctionsFilename} does not exist. Cannot use measuresuite if there is no so for check the functions' results against.`,
      );
    }
    if (bounds.length === 0) {
      // default to 'use all bits'
      bounds = Array(argwidth).fill("0xffffffffffffffff");
    }
    if (argwidth !== bounds.length) {
      throw new Error("Illegal arguments. Bounds array must be same size as argwidth, or empty.");
    }
    const bigInts = bounds.map((v) => {
      if (["0", "2", "4", "8"].some((end) => v.endsWith(end))) {
        return BigInt(v) - BigInt(1);
      }
      return BigInt(v);
    });
    const bounds_u64 = new BigUint64Array(bigInts);

    try {
      ms.measuresuite_init(
        argwidth,
        argNumIn,
        argNumOut,
        chunkSize,
        bounds_u64,
        Measuresuite.libcheckfunctionsFilename,
        functionSymbol,
      );
    } catch (e) {
      console.error(e);
      throw new Error("Could not initialize Measuresuite.");
    }
  }

  public measure(
    functionA: string,
    functionB: string,
    batchSize: number,
    numBatches: number,
  ): MeasureResult | null {
    let result: string | undefined;
    try {
      result = ms.measuresuite_measure(functionA, functionB, batchSize, numBatches);
    } catch (e) {
      console.error("Measuresuite: in measuresuite_measure, an error occurred", e);
      throw new Error(`Could not measure.${e}`);
    }
    // const sub = result.substring( result.indexOf("{"), result.lastIndexOf("}") + 1); // skip other output, e.g. wrong bytes or extend memory info
    if (result) {
      try {
        return JSON.parse(result) as MeasureResult;
      } catch (e) {
        console.error(`While JSON.parse'ing an error occurred: I tried to parse: >>${result}<<`);
        throw e;
      }
    }
    return null;
  }
}
