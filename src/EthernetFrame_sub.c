#include "dds/dds.h"
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include "EthernetFrame.h"


int main(int argc, char *argv[]) {
	if (argc != 2) {
		fprintf(stderr, "Usage: %s <sub_topic_name>\n", argv[0]);
		return EXIT_FAILURE;
	}
	char *sub_topic_name = argv[1];


	printf("=== [Subscriber] Starting ===\n");

		// Participant
		const dds_entity_t kParticipant = dds_create_participant(DDS_DOMAIN_DEFAULT, NULL, NULL);
		if (kParticipant < 0) {
			fprintf(stderr, "Failed to create participant! Error: %d\n", kParticipant);
			return -1;
		}

		// Create a topic for subscription
		dds_entity_t sub_topic = dds_create_topic(kParticipant, &ethernet_msgs_msg_EthernetFrame_desc, sub_topic_name, NULL, NULL);
		if (sub_topic < 0) {
			fprintf(stderr, "Failed to create topic! Error: %d\n", sub_topic);
			dds_delete(kParticipant);
			return -1;
		}

		// QoS & Reader
		dds_qos_t* qos = dds_create_qos();
		dds_qset_reliability(qos, DDS_RELIABILITY_BEST_EFFORT, DDS_SECS(10));
		const dds_entity_t kReader = dds_create_reader(kParticipant, sub_topic, qos, NULL);
		dds_delete_qos(qos);
		if (kReader < 0) {
			fprintf(stderr, "Failed to create reader! Error: %d\n", kReader);
			dds_delete(sub_topic);
			dds_delete(kParticipant);
			return -1;
		}

		// Allocate memory for samples
#define K_MAX_SAMPLES 1
		ethernet_msgs_msg_EthernetFrame* sample = ethernet_msgs_msg_EthernetFrame__alloc();
		void* samples[K_MAX_SAMPLES] = { sample };
		dds_sample_info_t infos[K_MAX_SAMPLES];
		

		// Main loop to receive messages
		while (true) {
			dds_return_t rc = dds_take(kReader, samples, infos, K_MAX_SAMPLES, K_MAX_SAMPLES);
			if (rc < 0) {
				fprintf(stderr, "Error during dds_take: return code %d\n", rc);
				break;
			}

			if (rc > 0) {
				for (uint32_t i = 0; i < rc; ++i) {
					if (infos[i].valid_data) {
						ethernet_msgs_msg_EthernetFrame* received_frame = (ethernet_msgs_msg_EthernetFrame*)samples[i];
						printf("Received Frame ID: %u\n",received_frame->header.stamp.nanosec);
					}
				}
			}
			else {
				dds_sleepfor(DDS_MSECS(100));
			}
		}

		dds_delete(kReader);
		dds_delete(sub_topic);
		dds_delete(kParticipant);

		printf("=== [Subscriber] Done ===\n");
		return 0;

	
	/*catch (const std::exception& e) {
		std::cerr << "=== [Subscriber] Exception: " << e.what() << std::endl;
		return EXIT_FAILURE;
	}*/
}