const express = require("express");
const router = express.Router();

const { checkRateLimit } = require("../controllers/rateLimiterController");

router.post("/allow", checkRateLimit);

module.exports = router;