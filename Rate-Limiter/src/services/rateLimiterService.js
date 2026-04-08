const { client } = require("../config/redisClient");
const { getRateLimitConfig } = require("../config/rateLimitConfig");

async function allowRequest(userId) {
    const key = `rate_limit:${userId}`;
    const now = Date.now();

    const { maxTokens, refillRate } = getRateLimitConfig(userId);

    const luaScript = `
    local data = redis.call("GET", KEYS[1])
    local maxTokens = tonumber(ARGV[1])
    local refillRate = tonumber(ARGV[2])
    local now = tonumber(ARGV[3])

    local tokens
    local lastRefill

    if not data then
        tokens = maxTokens
        lastRefill = now
    else
        local decoded = cjson.decode(data)
        tokens = decoded.tokens
        lastRefill = decoded.lastRefill
    end

    local timePassed = (now - lastRefill) / 1000
    tokens = math.min(maxTokens, tokens + timePassed * refillRate)

    local allowed = 0
    if tokens >= 1 then
        tokens = tokens - 1
        allowed = 1
    end

    local newData = cjson.encode({
        tokens = tokens,
        lastRefill = now
    })

    redis.call("SET", KEYS[1], newData, "EX", 60)

    return { allowed, tokens, lastRefill }
    `;

    const result = await client.eval(luaScript, {
        keys: [key],
        arguments: [
            maxTokens.toString(),
            refillRate.toString(),
            now.toString()
        ]
    });

    const allowed = result[0] === 1;
    const remaining = Math.floor(result[1]);

    const resetTime = Math.ceil(
        (maxTokens - result[1]) / refillRate
    );

    return {
        allowed,
        remaining,
        resetTime,
        maxTokens
    };
}

module.exports = { allowRequest };