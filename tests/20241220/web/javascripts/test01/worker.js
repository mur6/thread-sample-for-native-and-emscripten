const add = (a, b) => a + b;

onmessage = (e) => {
    const { funcName, input } = e.data;
    console.log(`recieved: funcName=${funcName}`);
    console.log(`recieved: input=${input}`);

    if (funcName === "add") {
        const num = add(input.a, input.b);
        console.log(`num=${num}`);
        postMessage({ result: num });
    } else {
        postMessage({ error: `unknown funcName=${funcName}` });
    }
};
