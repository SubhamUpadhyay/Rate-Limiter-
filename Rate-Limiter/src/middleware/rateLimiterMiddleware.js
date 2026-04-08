const { allowRequest } = require("../services/rateLimiterService");

async function rateLimiterMiddleware(req, res, next) {
    try {
        const userId = req.body.userId || req.ip;

        if (!userId) {
            return res.status(400).json({ error: "userId required" });
        }

        const result = await allowRequest(userId);

        res.set({
            "X-RateLimit-Limit": result.maxTokens,
            "X-RateLimit-Remaining": result.remaining,
            "X-RateLimit-Reset": result.resetTime
        });

        if (!result.allowed) {
            return res.status(429).json({
                error: "Too many requests"
            });
        }

        next();
    } catch (error) {
        console.error(error);
        return res.status(500).json({ error: "Internal Server Error" });
    }
}

module.exports = rateLimiterMiddleware;