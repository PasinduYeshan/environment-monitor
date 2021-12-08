const mysql = require("mysql");
const _pool = new WeakMap();
const _connectionError = new WeakMap();
const _getResults = new WeakMap();

class Database {
  constructor() {
    try {
      _pool.set(
        this,
        mysql.createPool({
          host: "localhost",
          user: "root",
          database: "emonitor",
          password: "246684",
        })
      );
      _connectionError.set(this, false);
    } catch (ex) {
      _connectionError.set(this, true);
    }

    _getResults.set(this, (error, results) => {
      if (error) {
        return { error: true };
      }
      return { error: false, result: results };
    });
  }

  get connectionError() {
    return _connectionError.get(this);
  }

  //Insert into sensor_data table
  insertData(values) {
    return new Promise((resolve) => {
      _pool.get(this).query(
        `INSERT INTO sensor_data (
          alert_id ,time_stamp , temperature,t_sd,humidity,h_sd, pressure, p_sd, light, l_sd ) VALUES (?)`,
        [values],
        (error, results, fields) => {
          console.log("Database error", error);
          resolve(_getResults.get(this)(error, results));
        }
      );
    });
  }

  //read from sensor_data table
  readTable() {
    return new Promise((resolve) => {
      _pool
        .get(this)
        .query(`SELECT * FROM sensor_data`, [], (error, results, fields) => {
          console.log("Database error", error);
          resolve(_getResults.get(this)(error, results));
        });
    });
  }
}

module.exports = Database;
