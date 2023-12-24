/*
 * mqqt.cpp
 *
 *  Created on: 24 dec. 2023
 *      Author: andre
 */

#include <cstdio>
#include <cstring>

#include "mqtt.hpp"
#include <mosquittopp.h>

int mqqt_test::connect_to_server(const char *id, const char *host, int port) {

	int keepalive = 60;
	mosquittopp(id, true);
	return connect(host, port, keepalive);
}

mqqt_test::~mqqt_test() {
}

void mqqt_test::on_connect(int rc) {
	fflush(stdout);
	printf("MQTT connected with code %d.\n", rc);
}

void mqqt_test::on_message(const struct mosquitto_message *message) {
	fflush(stdout);
	printf("MQTT message received\n");

}

void mqqt_test::on_subscribe(int mid, int qos_count, const int *granted_qos) {
	fflush(stdout);
	printf("MQTT subscribed\n");

}

void mqqt_test::on_publish(int mid) {
	printf("MQTT message published, mid %d\n",mid);
};

int mqqt_test::publish_sensorvalue(int sensor_id, int sensor_type,
		float value) {
	char topic[256];
	snprintf(topic, sizeof(topic), "blaatschaap/sensor/%d/%d", sensor_id,
			sensor_type);
	char message[256];
	snprintf(message, sizeof(message), "%f", value);
	int mid = 0;
	int result = publish(&mid, topic, strlen(message), message);
	printf("Publish message mid %d status %d\n",mid,result);
	return result;
}

int mqqt_test::publish_sensorvalue(int node_id, int sensor_id, const char* sensor_type, const char * sensor_value){
	char topic[256];
	snprintf(topic, sizeof(topic), "blaatschaap/sensor/%d/%d/%s", node_id,
			sensor_id, sensor_type);
	int mid = 0;
	int result = publish(&mid, topic, strlen(sensor_value), sensor_value);
	printf("Publish message mid %d status %d\n",mid,result);
	return result;
}
