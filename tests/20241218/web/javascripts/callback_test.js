// use comlink to run heavyCalc() in a worker

console.log('callback_test.js loaded');

// function fib(n) {
//     if (n < 2) {
//         return n;
//     }
//     return fib(n - 1) + fib(n - 2);
// }

// memoized version
const memo = new Map();

function fib(n) {
    if (n < 2) {
        return n;
    }
    if (memo.has(n)) {
        return memo.get(n);
    }
    const result = fib(n - 1) + fib(n - 2);
    memo.set(n, result);
    return result;
}

function heavyCalc(n) {
    return fib(n);
}

const run = async () => {
    const time = new Date().getTime();
    console.log("heavyCalc(): start");
    const result = heavyCalc(45);
    const elapsed = new Date().getTime() - time;
    console.log(`heavyCalc(): end, result=${result}, elapsed=${elapsed}ms`);
}

run();
