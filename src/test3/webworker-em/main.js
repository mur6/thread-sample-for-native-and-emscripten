var calcId = 0;



// 使用例
async function runCalculations() {
    const inputNumberStr = document.getElementById("input").value;
    const inputNumber = parseInt(inputNumberStr, 10);
    try {
        // 計算開始時間を記録
        const startTime = performance.now();
        console.log(`計算${calcId}開始...`);
        // clear output text
        document.getElementById("output").textContent = "[calc...]";
        const result = await calculateInWorker(1, inputNumber); // 関数1の計算
        // 計算終了時間を記録
        const endTime = performance.now();
        console.log(`計算${calcId}終了. 計算時間: ${endTime - startTime}ms`);
        calcId++;

        document.getElementById("output").textContent = `計算結果: ${result}`;
    } catch (error) {
        console.error("エラーが発生しました:", error);
    }
}

// ボタンのクリックイベントにバインド
document.getElementById("start").addEventListener("click", runCalculations);

var simpleCountNumber = 0;
document.getElementById("countup").addEventListener("click", () => {
    document.getElementById("countup-output").textContent = `Count: ${simpleCountNumber++}`;
});
