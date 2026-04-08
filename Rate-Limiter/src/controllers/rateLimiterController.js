const { allowRequest } = require("../services/rateLimiterService");

async function checkRateLimit(req, res) {
    try {
        const { userId } = req.body;

        if (!userId) {
            return res.status(400).json({ error: "userId required" });
        }

        const allowed = await allowRequest(userId);

        return res.json({ allowed });
    } catch (error) {
        console.error(error);
        return res.status(500).json({ error: "Internal Server Error" });
    }
}

module.exports = { checkRateLimit };