const SensorData = require("./model.js");
const path = require("path");
const express = require("express");

const app = express();
app.set("view engine", "ejs");
app.set("views", path.join(__dirname, "views"));
app.use(express.static(path.join(__dirname, "../public")));
app.use(express.static(path.join(__dirname, "../node_modules/bootstrap/dist")));
app.use(express.json());
app.use(express.urlencoded({ extended: true }));

const port = process.env.PORT || 3000;
const server = app.listen(port, () => console.log(`Listening on port ${port}`));

/**
 * View web page
 */
app.get("/", async (req, res) => {
  const result = await SensorData.getAllSensorData();
  // If there is error with database send 500 internal server error
  if (result.error) {
    res.status(500).send();
  }
  console.log(result.result);
  res.status(200).send(res.result);
});

module.exports = server;
