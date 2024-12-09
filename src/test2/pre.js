function test_pre_js() {
    console.log('test_pre_js');
}


function wrapAsyncCalculationWithPromise(asyncCalculation) {
    return function(input) {
        return new Promise((resolve, reject) => {
            try {
                asyncCalculation(input, (error, result) => {
                    if (error) {
                        reject(error);
                    } else {
                        resolve(result);
                    }
                });
            } catch (error) {
                reject(error);
            }
        });
    };
}

// // 使用例
// function originalAsyncCalculation(input, callback) {
//     // 元の非同期関数の例
//     setTimeout(() => {
//         if (input < 0) {
//             callback(new Error('Invalid input'), null);
//         } else {
//             callback(null, input * 2);
//         }
//     }, 1000);
// }
