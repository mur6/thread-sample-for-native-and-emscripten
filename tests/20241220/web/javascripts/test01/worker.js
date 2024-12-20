const add = (a, b) => a + b;

onmessage = async function (e) {
    const { funcName, input } = e.data;
    console.log(`recieved: funcName=${funcName}`);
    console.log(`recieved: input=${input}`);

    if (funcName === "add") {
        const result = add(input.a, input.b);
        postMessage({ result: result });
    } else {
        postMessage({ error: `unknown funcName=${funcName}` });
    }
};
