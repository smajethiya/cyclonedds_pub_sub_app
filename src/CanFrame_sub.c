#include "dds/dds.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "CanFrame.h" // Adjust the include path as necessary

int main(int argc, char* argv[]) {
	if (argc != 2) {
		fprintf(stderr, "Usage: %s <sub_topic_name>\n", argv[0]);
		return EXIT_FAILURE;
	}

	char* sub_topic_name = argv[1];
	dds_entity_t participant = DDS_ENTITY_NIL;
	dds_entity_t topic = DDS_ENTITY_NIL;
	dds_entity_t reader = DDS_ENTITY_NIL;
	dds_qos_t* qos = NULL;

		printf("=== [Subscriber] Starting ===\n");

		// Create a DomainParticipant
		participant = dds_create_participant(DDS_DOMAIN_DEFAULT, NULL, NULL);
		if (participant < 0) {
			fprintf(stderr, "Failed to create participant! Error: %d\n", participant);
			return EXIT_FAILURE;
		}

		// Create a Topic
		topic = dds_create_topic(participant, &can_msgs_msg_CanFrame_desc, sub_topic_name, NULL, NULL);
		if (topic < 0) {
			fprintf(stderr, "Failed to create topic! Error: %d\n", topic);
			dds_delete(participant);
			return EXIT_FAILURE;
		}

		// Create QoS and set reliability
		qos = dds_create_qos();
		dds_qset_reliability(qos, DDS_RELIABILITY_RELIABLE, DDS_SECS(10));
		dds_qset_history(qos, DDS_HISTORY_KEEP_ALL, 0);
		dds_qset_resource_limits(qos, DDS_LENGTH_UNLIMITED, DDS_LENGTH_UNLIMITED, DDS_LENGTH_UNLIMITED);


		// Create a DataReader
		reader = dds_create_reader(participant, topic, qos, NULL);
		
		if (reader < 0) {
			fprintf(stderr, "Failed to create reader! Error: %d\n", reader);
			dds_delete(topic);
			dds_delete(participant);
			return EXIT_FAILURE;
		}

		// Allocate memory for samples
#define K_MAX_SAMPLES 1
		can_msgs_msg_CanFrame* sample = can_msgs_msg_CanFrame__alloc();
		void* samples[K_MAX_SAMPLES] = { sample };
		dds_sample_info_t infos[K_MAX_SAMPLES];

		// Main loop to receive messages
		while (true) {
			dds_return_t rc = dds_take(reader, samples, infos, K_MAX_SAMPLES, K_MAX_SAMPLES);
			if (rc < 0) {
				fprintf(stderr, "Error during dds_take: return code %d\n", rc);
				break;
			}

			if (rc > 0) {
				for (uint32_t i = 0; i < rc; ++i) {
					if (infos[i].valid_data) {
						can_msgs_msg_CanFrame* received_frame = (can_msgs_msg_CanFrame*)samples[i];
						printf("Received Frame ID: %u\n", received_frame->header.stamp.sec);
						// Access other fields of `received_frame` as needed
					}
				}
			}
			else {
				dds_sleepfor(DDS_MSECS(100)); // Sleep if no data is received
			}
		}

		// Clean up
		can_msgs_msg_CanFrame_free(sample, DDS_FREE_ALL);
		
		dds_delete_qos(qos); 
		dds_delete(reader);
		dds_delete(topic);
		dds_delete(participant);

		printf("=== [Subscriber] Done ===\n");
		return EXIT_SUCCESS;

	
	/*catch (const std::exception& e) {
		fprintf(stderr, "Exception occurred: %s\n", e.what());
		if (reader != DDS_ENTITY_NIL) dds_delete(reader);
		if (topic != DDS_ENTITY_NIL) dds_delete(topic);
		if (participant != DDS_ENTITY_NIL) dds_delete(participant);
		return EXIT_FAILURE;
	}*/
}
