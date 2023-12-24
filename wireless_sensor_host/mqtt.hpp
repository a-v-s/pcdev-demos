/*
 * mqqt.hpp
 *
 *  Created on: 24 dec. 2023
 *      Author: andre
 */

#ifndef MQTT_HPP_
#define MQTT_HPP_



#include <mosquittopp.h>

class mqqt_test : public mosqpp::mosquittopp
{
	public:
		int connect_to_server(const char *id, const char *host, int port);
		~mqqt_test();

		void on_connect(int rc);
		void on_message(const struct mosquitto_message *message);
		void on_subscribe(int mid, int qos_count, const int *granted_qos);
		void on_publish(int mid);



		int publish_sensorvalue(int sensor_id, int sensor_type, float value);

		int publish_sensorvalue(int node_id, int sensor_id, const char* sensor_type, const char * sensor_value);

};


#endif /* MQTT_HPP_ */
