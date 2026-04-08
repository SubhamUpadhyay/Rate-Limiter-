const { createClient } = require("redis");

const client = createClient({
    url: "redis://127.0.0.1:6379"
});

client.on("error", (err) => {
    console.error("Redis Client Error", err);
});

async function connectRedis() {
    await client.connect();
    console.log("Connected to Redis");
}

module.exports = { client, connectRedis };