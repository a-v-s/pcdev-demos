/*
 * mqqt.cpp
 *
 *  Created on: 24 dec. 2023
 *      Author: andre
 */

#include <cstdio>
#include <cstring>
#include <thread>

#include "mqtt.hpp"
#include <mosquittopp.h>
#include <nlohmann/json.hpp>

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
	printf("MQTT message published, mid %d\n", mid);
}
;

int mqqt_test::publish_sensorvalue(int sensor_id, int sensor_type,
		float value) {
	char topic[256];
	snprintf(topic, sizeof(topic), "blaatschaap/sensor/%d/%d", sensor_id,
			sensor_type);
	char message[256];
	snprintf(message, sizeof(message), "%f", value);
	int mid = 0;
	int result = publish(&mid, topic, strlen(message), message);
	printf("Publish message mid %d status %d\n", mid, result);
	return result;
}

int mqqt_test::publish_sensorvalue(int node_id, int sensor_id,
		const char *sensor_type, const char *sensor_value) {
	char topic[256];
	snprintf(topic, sizeof(topic), "blaatschaap/sensor/%d/%d/%s", node_id,
			sensor_id, sensor_type);
	int mid = 0;
	int result = publish(&mid, topic, strlen(sensor_value), sensor_value);
	printf("Publish message mid %d status %d\n", mid, result);
	return result;
}

int mqqt_test::publish_sensorvalue(int node_id, int sens_id,
		const char *device_class, const char *value,
		const char *unit_of_measurement) {
	char config_topic[256];
	char state_topic[256];
	nlohmann::json json_config;
	nlohmann::json json_value;

	snprintf(config_topic, sizeof(config_topic),
			"homeassistant/sensor/unit_%02x/sensor_%02x_%s/config",
			node_id, sens_id, device_class);
	snprintf(state_topic, sizeof(state_topic),
			"homeassistant/sensor/unit_%02x/sensor_%02x_%s/value",
				node_id, sens_id, device_class);

	json_config["device_class"] = device_class;
	json_config["state_topic"] = state_topic;
	json_config["unit_of_measurement"] = unit_of_measurement;
	json_config["value_template"] = "{{value}}";

	char unique_id_buff[32];
	snprintf(unique_id_buff, sizeof(unique_id_buff), "BS%02x%02x%s",
			node_id & 0xFF, sens_id & 0xFF, device_class);
	char name_buff[16];
	snprintf(name_buff, sizeof(name_buff), "BS%02x%02x",
				node_id & 0xFF, sens_id & 0xFF);
	json_config["unique_id"] = unique_id_buff;
	json_config["device"]["name"] = name_buff;
	json_config["device"]["identifiers"] = name_buff;
	json_config["device"]["model"] = "Blaat";



	int mid;
	auto json_config_dump = json_config.dump();
	int result = publish(&mid, config_topic, json_config_dump.length(),
			json_config_dump.c_str());
	printf("Publish message mid %d status %d\n", mid, result);

	json_value = value;
	auto json_value_dump = json_value.dump();
//	std::this_thread::sleep_for(std::chrono::seconds(1));
	result = publish(&mid, state_topic, strlen(value), value);
	//result = publish(&mid, state_topic, json_value_dump.length(), json_value_dump.c_str());
	printf("Publish message mid %d status %d\n", mid, result);

	return result;

}


int mqqt_test::publish_sensorvalue(int node_id, int sens_id,
		const char *device_class, float value,
		const char *unit_of_measurement) {
	char strvalue[16];
	snprintf(strvalue,sizeof(strvalue),"%f",value);
	return publish_sensorvalue(node_id,sens_id, device_class,strvalue, unit_of_measurement);
}
