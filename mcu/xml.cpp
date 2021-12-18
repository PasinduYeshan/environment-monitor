#include <Arduino.h>

void generate_xml(char *xml,
                    double temperature, double humidity, double pressure, double light,
                    double temperature_sd, double humidity_sd, double pressure_sd, double light_sd,
                    String identifier, String timestamp)
{
    sprintf(
        xml,
        "<?xml version = \"1.0\" encoding = \"utf8\"?>"
        "<alert>"
        "<alertId>%s</alertId>"
        "<sender> Environment Monitor</sender>"
        "<timeStamp> %s</timeStamp>"
        "<status> Actual</status>"
        "<msgType> Alert</msgType>"
        "<scope> Public</scope>"

        "<info>"
        "<category> Env</category>"
        "<event> Readings</event>"
        "<urgency> Past</urgency>"
        "<severity> Minor</severity>"
        "<certainty> Observed</certainty>"
        "<senderName> Environment Monitor</senderName>"

        "<parameter><valueName>%s</valueName><value>%s</value></parameter>"
        "<parameter><valueName>%s</valueName><value>%s</value></parameter>"
        "<parameter><valueName>%s</valueName><value>%s</value></parameter>"
        "<parameter><valueName>%s</valueName><value>%s</value></parameter>"

        "<parameter><valueName>%s</valueName><value>%s</value></parameter>"
        "<parameter><valueName>%s</valueName><value>%s</value></parameter>"
        "<parameter><valueName>%s</valueName><value>%s</value></parameter>"
        "<parameter><valueName>%s</valueName><value>%s</value></parameter>"

        "</info>",

        identifier.c_str(), timestamp.c_str(),
        "temperature", String(temperature).c_str(),
        "humidity", String(humidity).c_str(),
        "pressure", String(pressure).c_str(),
        "li", String(light).c_str(),

        "temperature_sd", String(temperature_sd).c_str(),
        "humidity_sd", String(humidity_sd).c_str(),
        "pressure_sd", String(pressure_sd).c_str(),
        "li_sd", String(light_sd).c_str()

    );
}