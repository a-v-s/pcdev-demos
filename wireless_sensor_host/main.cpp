#include "Device.hpp"
#include "DeviceManager.hpp"




#include "mqtt.hpp"


static DeviceManager m_dm;
static mqqt_test* mp_mqtt;

extern "C" {
#include "protocol.h"
#include "sensor_protocol.h"

bscp_handler_status_t forward_handler(bscp_protocol_packet_t *data,
		protocol_transport_t transport, uint32_t param) {
	bscp_protocol_forward_t *forwarded_data =
			(bscp_protocol_forward_t*) (data->data);
	protocol_parse(forwarded_data->data, data->head.size, transport,
			forwarded_data->from);
	return BSCP_HANDLER_STATUS_OK;
}

bscp_handler_status_t sensordata_handler(bscp_protocol_packet_t *packet,
		protocol_transport_t transport, uint32_t param) {
	bsprot_sensor_enviromental_data_t *sensordata =
			(bsprot_sensor_enviromental_data_t*) packet->data;
	printf("Sensor %2d ", sensordata->id);

	if (param!= 1 ) {
		puts("Unexpeced node id");
	}

	const char * sensortype = nullptr;
	char sensorvalue[16];
	switch (sensordata->type) {
	case bsprot_sensor_enviromental_temperature:
		sensortype =  "temperature";
		snprintf(sensorvalue, sizeof(sensorvalue), "%6.2f °C",
				(float) (sensordata->value.temperature_centi_celcius) / 100.0f);



		break;
	case bsprot_sensor_enviromental_humidity:
		sensortype= "humidity";
				snprintf(sensorvalue, sizeof(sensorvalue), "%6.1f  %%",		(float) (sensordata->value.humidify_relative_promille) / 10.0f);


		break;
	case bsprot_sensor_enviromental_illuminance:
		sensortype = "illuminance";
		snprintf(sensorvalue, sizeof(sensorvalue), "%7.0f lux",
				(float)sensordata->value.illuminance_lux);

		break;
	case bsprot_sensor_enviromental_airpressure:
		sensortype = "airpressure";
		snprintf(sensorvalue, sizeof(sensorvalue), "%6.1f  hPa",
				(float) (sensordata->value.air_pressure_deci_pascal) / 10.0f);

		break;

	case bsprot_sensor_enviromental_co2:
		sensortype = "CO₂";
		snprintf(sensorvalue, sizeof(sensorvalue),"%7.0f ppm",  (float)sensordata->value.co2_ppm);

		break;
	case bsprot_sensor_enviromental_eco2:
		sensortype = "eCO₂";
		snprintf(sensorvalue, sizeof(sensorvalue),"%7.0f ppm",  (float)sensordata->value.eco2_ppm);

		break;
	case bsprot_sensor_enviromental_etvoc:
		sensortype = "eTVOC";
		snprintf(sensorvalue, sizeof(sensorvalue),"%7.0f ppb",  (float)sensordata->value.etvoc_ppb);

		break;
	case bsprot_sensor_enviromental_pm25:
		sensortype = "PM2.5";
		snprintf(sensorvalue, sizeof(sensorvalue),"%7.0f µg/m³", (float)sensordata->value.pm25_ugm3);

		break;
	default:
		printf("Unknown sensor type %02X\n", sensordata->type);
		return BSCP_HANDLER_STATUS_BADDATA;
		break;

	}

	printf("%3d %16s %16s\n", sensordata->id, sensortype, sensorvalue);
	putchar('\n');

	mp_mqtt->publish_sensorvalue(param,sensordata->id, sensortype, sensorvalue);
	return BSCP_HANDLER_STATUS_OK;
}
}


int main(int argc, char *argv[]) {
	protocol_register_command(forward_handler, BSCP_CMD_FORWARD);
	protocol_register_command(sensordata_handler,
			BSCP_CMD_SENSOR_ENVIOREMENTAL_VALUE);

	mosqpp::lib_init();
	mp_mqtt = new mqqt_test();
	//int result = mp_mqtt->connect_to_server("1234", "test.mosquitto.org", 1883);
	int result = mp_mqtt->connect("test.mosquitto.org", 1883);
	printf("mqtt connect returned %d\n", result);

	m_dm.start();
	while (1)
		;

	mosqpp::lib_cleanup();
}
