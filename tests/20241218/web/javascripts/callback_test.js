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

function setTimeoutPromise(ms) {
    return new Promise((resolve) => {
        setTimeout(() => {
            resolve();
        }, ms);
    });
}

const run = async () => {
    // log current time
    console.log(`run(): ${new Date().toISOString()}`);
    await setTimeoutPromise(3000);
    // print log with await
    console.log('run(): after 1s');
}

run();
