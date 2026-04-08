function getRateLimitConfig(userId) {
    if (userId.startsWith("premium")) {
        return {
            maxTokens: 10,
            refillRate: 5
        };
    }

    if (userId.startsWith("admin")) {
        return {
            maxTokens: 1000,
            refillRate: 100
        };
    }

    return {
        maxTokens: 3,
        refillRate: 0.5
    };
}

module.exports = { getRateLimitConfig };