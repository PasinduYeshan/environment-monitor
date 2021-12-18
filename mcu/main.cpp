#include <Arduino.h>
#include <WiFi.h>
#include <queue>
#include "xml.h"
using namespace std;

/**
 * @brief Configs
 * 
 */
 const char *SSID = "WIFI_SSID";
const char *PW = "password";
const char *serverBase = "http://localhost:3000/";
const char *ntpServer = "pool.ntp.org";
const long gmtOffsetInSeccond = 19800;
const int dayLightOffsetInSecond = 0;

/**
 * @brief Initialize values to 0
 * 
 */
int msg = 0;
double temperature = 0;
double humidity = 0;
double pressure = 0;
double light = 0;
double temperature_sd = 0;
double humidity_sd = 0;
double pressure_sd = 0;
double light_sd = 0;

char xml[1700];
String identifier;
char datetime_[32] = {};
String datetime;

/**
 * @brief Buffer to store identifiers, timestamp and message Ids.
 * 
 */
queue<String> bf_id;
queue<String> bf_timestamp;
queue<int> bf_msg_ids;

/**
 * @brief Buffer to store mean values of temperature,
  humidity , pressure and light
 */
queue<double> bf_t;
queue<double> bf_h;
queue<double> bf_p;
queue<double> bf_l;

/**
 * @brief Buffer to store standard deviation values of temperature,
  humidity , pressure and light
 */
queue<double> bf_t_;
queue<double> bf_h_;
queue<double> bf_p_;
queue<double> bf_l_;

/**
 * @brief Get the Local Time Stamp
 * 
 * @param datetime
 */

void get_timestamp(char *datetime_)
{
  struct tm timeinfo;
  if (!getLocalTime(&timeinfo))
  {
    Serial.println("Failed to obtain time");
  }
  Serial.print("[TIME] ");

  char timeString[32];
  time_t timeT = time(NULL);
  strftime(timeString, sizeof(timeString), "%Y-%m-%dT%H:%M:%S%z", localtime(&timeT));
  printf("%s\n", timeString);
  sprintf(datetime_, timeString);
}

/**
 * @brief Pop buffer
 * 
 */
void remove_buffer_data()
{
  bf_msg_ids.pop();
  bd_id.pop();
  bf_timestamp.pop();

  bf_t.pop();
  bf_h.pop();
  bf_p.pop();
  bf_l.pop();

  bf_t_sd.pop();
  bf_h_sd.pop();
  bf_p_sd.pop();
  bf_l_sd.pop();
}

/**
 * @brief Push data to buffer
 * 
 */
void push_buffer_values()
{
  bf_msg_ids.push(msg);
  bd_id.push(identifier);
  bf_timestamp.push(datetime);

  bf_t.push(temperature);
  bf_h.push(humidity);
  bf_p.push(pressure);
  bf_l.push(light);

  bf_t_sd.push(temperature_sd);
  bf_h_sd.push(humidity_sd);
  bf_p_sd.push(pressure_sd);
  bf_l_sd.push(light_sd);
}

/**
 * @brief Wait and try to connect Wifi
 * 
 */
void connect_wifi_reconnect()
{
    WiFi.begin(SSID, PW);
    while (WiFi.status() != WL_CONNECTED)
    {
        delay(1000);
    }
}

/**
 * @brief Check wifi connection
 * 
 * @return true 
 * @return false 
 */
bool connect_to_wifi()
{
    WiFi.begin(SSID, PW);
    if (WiFi.status() == WL_CONNECTED)
    {
      return true;
    }
    return false;
}

/**
 * @brief Calculate mean value
 * 
 * @param arr 
 * @param len 
 * @return double 
 */
double calculate_mean(double arr[], int len)
{
    double tot = 0;
    for (int i = 0; i < len; i++)
    {
        tot += arr[i];
    }
    double mean = round((tot / len) * 100) / 100.00;
    return mean;
}

/**
 * @brief Calculate standard deviation
 * 
 * @param arr 
 * @param len 
 * @param mean 
 * @return double 
 */
double calculate_sd(double arr[], int len, double mean)
{
    double variance = 0;
    for (int i = 0; i < len; i++)
    {
        variance += pow((arr[i] - mean), 2);
    }
    double sd = round(sqrt(variance) * 100) / 100.00;
    return sd;
}

/**
 * @brief Send data to the back end
 * 
 * @param xmlstr 
 * @param msg 
 * @return true 
 * @return false 
 */
bool send_data_to_server(char *xmlstr, int msg)
{
    // If wifi is not connected return false
    if (WiFi.status() != WL_CONNECTED)
    {
        return false;
    }

    // Send POST request
    HTTPClient http;
    String url = (String)serverBase + "/sensor-values";
    http.begin((String)url);
    int responseCode = http.POST(xmlstr);
    http.end();

    // If connection failed return false
    if (responseCode < 0)
    {
        return false;
    }
    // If server return status code other than 200
    if (responseCode != HTTP_CODE_OK)
    {
        return false;
    }
    return true;
}

/**
 * @brief Setup code
 * 
 */
void setup()
{
  Serial.begin(115200);
  connect_wifi_reconnect();
  begin_sensors();
  configTime(gmtOffsetInSeccond, dayLightOffsetInSecond, ntpServer);
}


void loop()
{
  while (!bf_msg_ids.empty())
  {
    if (WiFi.status() == WL_CONNECTED)
    {
      char xmlchar_buf[1700];

      int msg_id_buf = bf_msg_ids.front();
      String identifier_buf = (String)bd_id.front();
      String datetime_buf = (String)bf_timestamp.front();

      double t_buf = bf_t.front();
      double h_buf = bf_h.front();
      double p_buf = bf_p.front();
      double l_buf = bf_l.front();

      double t_sd_buf = bf_t_sd.front();
      double h_sd_buf = bf_h_sd.front();
      double p_sd_buf = bf_p_sd.front();
      double l_sd_buf = bf_l_sd.front();

      generate_xml(xmlchar_buf,
                     t_buf, h_buf, p_buf, l_buf,
                     t_sd_buf, h_sd_buf, p_sd_buf, l_sd_buf,
                     identifier_buf, datetime_buf);
      
      
      // If sending data is successfull remove data from buffer
      if (send_data_to_server(xmlchar_buf, msg_id_buf))
      {
        remove_buffer_data();
      }
      else
      {
        Serial.printf("Sending data to server failed");
        break;
      }
    }
    else
    {
      break;
    }
  }

  int x = 0;
  int rounds = 15;
  int round_time = 2000;
  
  double t_[rounds];
  double h_[rounds];
  double p_[rounds];
  double l_[rounds];

  // Read sensor data
  while (x < rounds)
  {
    t_[x] = round(readTemperature() * 100) / 100.00;
    h_[x] = round(readHumidity() * 100) / 100.00;
    p_[x] = round(readPressure() * 100) / 100.00;
    l_[x] = round(readLightIntensity() * 100) / 100.00;
    x++;
  }

  temperature = calculate_mean(t_, rounds);
  humidity = calculate_mean(h_, rounds);
  pressure = calculate_mean(p_, rounds);
  light = calculate_mean(l_, rounds);

  temperature_sd = calculate_sd(t_, rounds, temperature);
  humidity_sd = calculate_sd(h_, rounds, humidity);
  pressure_sd = calculate_sd(p_, rounds, pressure);
  light_sd = calculate_sd(l_, rounds, light);

  identifier = String(msg);
  get_timestamp(datetime_);
  datetime = String(datetime_);

  // Generate xml to xml variable
  generate_xml(xml,
                 temperature, humidity, pressure, light,
                 temperature_sd, humidity_sd, pressure_sd, light_sd,
                 identifier, datetime);
  
  /**
   * @brief If wifi is connected try to send dat to server
   If fails push sensor values to buffer
   If wifi is not connected try to connect to wifi
   Then send data to the server
   If fails push sensor values to buffer
   * 
   */
  if (WiFi.status() == WL_CONNECTED)
  {
    if (!send_data_to_server(xml, msg))
    {
      push_buffer_values();
    }
  }
  else
  {
    if (connect_to_wifi())
    {
      
      if (!send_data_to_server(xml, msg))
      {
        push_buffer_values();
      }
    }
    else
    {
      push_buffer_values();
    }
  }
  msg++;
}


