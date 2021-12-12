const SensorData = require("./model.js");
const path = require("path");
const express = require("express");
let ejs = require("ejs");

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
    return;
  }
  const t = result.result[0].time_stamp;
  console.log(result.result);
  res.status(200).render("./index.ejs", { values: result.result });
});

module.exports = server;
