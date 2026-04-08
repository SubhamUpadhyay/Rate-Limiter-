const express = require("express");
const { connectRedis } = require("./config/redisClient");
const rateLimiterMiddleware = require("./middleware/rateLimiterMiddleware");

const app = express();

app.use(express.json());

// Health check route (always keep this BEFORE limiter)
app.get("/", (req, res) => {
    res.send("Rate Limiter Service Running 🚀");
});

// Apply rate limiter safely
app.use("/api", async (req, res, next) => {
    try {
        await rateLimiterMiddleware(req, res, next);
    } catch (err) {
        console.error("Rate limiter error:", err);
        return res.status(500).json({
            error: "Internal Server Error (Rate Limiter Failure)"
        });
    }
});

// Test route
app.post("/api/test", (req, res) => {
    res.set({
        "X-Service": "Rate-Limiter",
    });

    res.json({ message: "Request successful ✅" });
});

const PORT = 3000;

// Start server only after Redis is ready
async function startServer() {
    try {
        await connectRedis();

        app.listen(PORT, () => {
            console.log(`Server running on port ${PORT}`);
        });

    } catch (err) {
        console.error("Failed to start server:", err);
        process.exit(1);
    }
}

startServer();