#include "dds/dds.h"
//#include "dds/dds.hpp"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "CanFrame.h" // Adjust the include path as necessary

int main(int argc, char* argv[]) {
	if (argc != 2) {
		fprintf(stderr, "Usage: %s <pub_topic_name>\n", argv[0]);
		return EXIT_FAILURE;
	}

	char* pub_topic_name = argv[1];
	dds_entity_t participant = DDS_ENTITY_NIL;
	dds_entity_t topic = DDS_ENTITY_NIL;
	dds_entity_t writer = DDS_ENTITY_NIL;
	dds_qos_t* qos = NULL;

		printf("=== [Publisher] Starting ===\n");

		// Create a DomainParticipant
		participant = dds_create_participant(DDS_DOMAIN_DEFAULT, NULL, NULL);
		if (participant < 0) {
			fprintf(stderr, "Failed to create participant! Error: %d\n", participant);
			return EXIT_FAILURE;
		}

		// Create a Topic
		topic = dds_create_topic(participant, &can_msgs_msg_CanFrame_desc, pub_topic_name, NULL, NULL);
		if (topic < 0) {
			fprintf(stderr, "Failed to create topic! Error: %d\n", topic);
			dds_delete(participant);
			return EXIT_FAILURE;
		}

		// Create QoS and set reliability
		qos = dds_create_qos();
		dds_qset_reliability(qos, DDS_RELIABILITY_BEST_EFFORT, DDS_SECS(10));

		// Create a DataWriter
		writer = dds_create_writer(participant, topic, qos, NULL);
		dds_delete_qos(qos);
		if (writer < 0) {
			fprintf(stderr, "Failed to create writer! Error: %d\n", writer);
			dds_delete(topic);
			dds_delete(participant);
			return EXIT_FAILURE;
		}

		// Allocate memory for a sample
		can_msgs_msg_CanFrame* sample = can_msgs_msg_CanFrame__alloc();
		if (!sample) {
			fprintf(stderr, "Failed to allocate memory for sample!\n");
			dds_delete(writer);
			dds_delete(topic);
			dds_delete(participant);
			return EXIT_FAILURE;
		}

		// Set dummy values for the CanFrame sample
		sample->header.stamp.nanosec = 123456789; // Example timestamp
		sample->id = 42;                         // Example CAN ID
		sample->is_rtr = false;
		sample->is_extended = false;
		sample->is_error = false;
		sample->is_can_fd = true;
		sample->is_constant_bitrate = true;
		sample->dlc = 8;                         // Example DLC
		memset(sample->data, 0xAB, sizeof(sample->data)); // Fill data with dummy value 0xAB

		// Publish the sample periodically
		for (int i = 0; i < 10; ++i) { // Publish 10 samples
			dds_return_t rc = dds_write(writer, sample);
			if (rc < 0) {
				fprintf(stderr, "Failed to publish sample! Error: %d\n", rc);
				break;
			}

			printf("Published Frame ID: %u\n", sample->id);
			dds_sleepfor(DDS_MSECS(1000)); // Wait for 1 second between publications
		}

		// Clean up
		can_msgs_msg_CanFrame_free(sample, DDS_FREE_ALL);
		dds_delete(writer);
		dds_delete(topic);
		dds_delete(participant);

		printf("=== [Publisher] Done ===\n");
		return EXIT_SUCCESS;

	
	/*catch (const std::exception& e) {
		fprintf(stderr, "Exception occurred: %s\n", e.what());
		if (writer != DDS_ENTITY_NIL) dds_delete(writer);
		if (topic != DDS_ENTITY_NIL) dds_delete(topic);
		if (participant != DDS_ENTITY_NIL) dds_delete(participant);
		return EXIT_FAILURE;
	}*/
}
