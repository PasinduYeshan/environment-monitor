const SensorData = require("./model.js");
const path = require("path");
const express = require("express");
let ejs = require("ejs");
var parseString = require("xml2js").parseString;

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

/**
 * Write to the database
 */
app.post("/sensor-values", async (req, res) => {
  try {
    parseString(req.body, function (err, result) {
      const sensorData = [
        result.alertId,
        result.timeStamp,
        result.temperature,
        result.temperature_sd,
        result.humidity,
        result.humidity_sd,
        result.pressure,
        result.pressure_sd,
        result.li,
        result.li_sd,
      ];
      await SensorData.writeSensorData(sensorData);
    });
    res.status(200).send();
  } catch (error) {
    res.status(500).send();
  }
});

module.exports = server;
