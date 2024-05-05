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

//int mqqt_test::connect_to_server(const char *id, const char *host, int port) {
//
//	int keepalive = 60;
//	mosquittopp(id, true);
//	return connect(host, port, keepalive);
//}

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

int mqqt_test::publish_sensor(int node_id, int sens_id,
		const char *device_class, const char *unit_of_measurement) {

	char config_topic[256];
	char state_topic[256];
	nlohmann::json json_config;
	nlohmann::json json_value;

	snprintf(config_topic, sizeof(config_topic),
			"homeassistant/sensor/unit_%02x/%s_%02x/config", node_id,
			device_class, sens_id);
	snprintf(state_topic, sizeof(state_topic),
			"homeassistant/sensor/unit_%02x/value", node_id);

	char value_template[256];
	snprintf(value_template, sizeof(value_template), "{{value_json.%s_%02x}}",
			device_class, sens_id);

	json_config["device_class"] = device_class;
	json_config["state_topic"] = state_topic;
	json_config["unit_of_measurement"] = unit_of_measurement;
	json_config["value_template"] = value_template;

	char unique_id[32];
	snprintf(unique_id, sizeof(unique_id), "sensor/%02X/%02X", node_id & 0xFF,
			sens_id & 0xFF);
	json_config["unique_id"] = unique_id;

	char name_buff[32];
	snprintf(name_buff, sizeof(name_buff), "%02X", node_id & 0xFF);
	json_config["device"]["identifiers"] = name_buff;

	int mid;
	auto json_config_dump = json_config.dump();
	int result = publish(&mid, config_topic, json_config_dump.length(),
			json_config_dump.c_str());
	printf("Publish configuration message mid %d status %d\n", mid, result);
	return result;

}

int mqqt_test::publish_sensorvalue(int node_id, int sens_id,
		const char *device_class, float value) {
	char state_topic[256];
	snprintf(state_topic, sizeof(state_topic),
			"homeassistant/sensor/unit_%02x/value", node_id);
	nlohmann::json json_value;
	char value_name[256];
	snprintf(value_name, sizeof(value_name), "%s_%02x", device_class, sens_id);
	json_value[value_name] = value;
	auto json_value_dump = json_value.dump();
	int mid;
	int result = publish(&mid, state_topic, json_value_dump.length(),
			json_value_dump.c_str());
	printf("Publish value message mid %d status %d\n", mid, result);
	return result;
}

int mqqt_test::publish_switch(int node_id, int switch_id) {

	char config_topic[256];
	char state_topic[256];
	char command_topic[256];
	nlohmann::json json_config;
	nlohmann::json json_value;

	snprintf(config_topic, sizeof(config_topic),
			"homeassistant/switch/unit_%02x/switch_%02x/config", node_id,
			switch_id);
	snprintf(state_topic, sizeof(state_topic),
			"homeassistant/switch/unit_%02x/value", node_id);

	snprintf(command_topic, sizeof(command_topic),
			"homeassistant/switch/unit_%02x/set/%02x", node_id, switch_id);

	json_config["state_topic"] = state_topic;
	json_config["command_topic"] = command_topic;

	json_config["payload_on"] = 1;
	json_config["payload_off"] = 0;
	json_config["state_on"] = 1;
	json_config["state_off"] = 0;

//	char value_template[256];
//	snprintf(value_template, sizeof(value_template),
//			"{{value_json.switch_%02x}}", switch_id);

	char unique_id[32];
	snprintf(unique_id, sizeof(unique_id), "switch/%02X/%02X", node_id & 0xFF,
			switch_id & 0xFF);
	json_config["unique_id"] = unique_id;

	char name_buff[32];
	snprintf(name_buff, sizeof(name_buff), "%02X", node_id & 0xFF);
	json_config["device"]["identifiers"] = name_buff;

	int mid;
	auto json_config_dump = json_config.dump();
	int result = publish(&mid, config_topic, json_config_dump.length(),
			json_config_dump.c_str());
	printf("Publish configuration message mid %d status %d\n", mid, result);
	return result;

}
