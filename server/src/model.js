const Database = require("./database.js");
const db = new Database();

class SensorData {
  /**
   * Get all the sensors data from the database
   */
  static async getAllSensorData() {
    const result = await db.readTable();
    return new Promise((resolve) => {
      let obj = {
        connectionError: db.connectionError,
      };
      if (result.error) {
        obj.error = true;
      } else {
        obj.error = false;
        obj.result = result.result;
      }
      resolve(obj);
    });
  }

  /**
   * Write sensor values to the database
   */
  static writeSensorData(sensorValues) {
    const result = db.readTable();
    return new Promise((resolve) => {
      let obj = {
        connectionError: db.connectionError,
      };
      if (result.error) {
        obj.error = true;
      } else {
        obj.error = false;
        obj.result = result.result;
      }
      resolve(obj);
    });
  }
}

module.exports = SensorData;
