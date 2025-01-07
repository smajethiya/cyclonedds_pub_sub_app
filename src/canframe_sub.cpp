#include "dds/dds.h"
#include "dds/dds.hpp"
#include <C:\Projects\CanFrame_build\CanFrame.h>


using namespace org::eclipse::cyclonedds;

int main(int argc, char *argv[]) {
	if (argc != 2) {
		std::cerr << "Usage: " << argv[0] << " <sub_topic_name>" << std::endl;
		return -1;
	}
	char *sub_topic_name = argv[1];

	try {
		std::cout << "=== [Subscriber] Created." << std::endl;

		// Participant
		const dds_entity_t kParticipant = dds_create_participant(DDS_DOMAIN_DEFAULT, nullptr, nullptr);
		if (kParticipant < 0) {
			std::cerr << "Failed to create participant!" << std::endl;
			return -1;
		}

		// Create a topic for subscription
		dds_entity_t sub_topic = dds_create_topic(kParticipant, &can_msgs_msg_CanFrame_desc, sub_topic_name, nullptr, nullptr);
		if (sub_topic < 0) {
			std::cerr << "Failed to create topic!" << std::endl;
			dds_delete(kParticipant);
			return -1;
		}

		// QoS & Reader
		dds_qos_t* qos = dds_create_qos();
		dds_qset_reliability(qos, DDS_RELIABILITY_BEST_EFFORT, DDS_SECS(10));
		const dds_entity_t kReader = dds_create_reader(kParticipant, sub_topic, qos, nullptr);
		dds_delete_qos(qos);
		if (kReader < 0) {
			std::cerr << "Failed to create DDS reader!" << std::endl;
			dds_delete(sub_topic);
			dds_delete(kParticipant);
			return -1;
		}

		const uint32_t kMaxSamples = 1;
		can_msgs_msg_CanFrame sample;
		void* samples[kMaxSamples] = { &sample };
		dds_sample_info_t infos[kMaxSamples];
		samples[0] = can_msgs_msg_CanFrame__alloc();

		while (true) {
			dds_return_t rc = dds_take(kReader, samples, infos, kMaxSamples, kMaxSamples);
			if (rc < 0) {
				std::cerr << "Error during dds_take:return code: " << rc << std::endl;
				break;
			}

			if (rc > 0) {
				for (uint32_t i = 0; i < rc; ++i) {
					if (infos[i].valid_data) {
						can_msgs_msg_CanFrame* received_frame = static_cast<can_msgs_msg_CanFrame*>(samples[i]);
						std::cout << "Received Frame ID: " << received_frame->header.stamp.nanosec << std::endl;
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

		std::cout << "=== [Subscriber] Done." << std::endl;
		return 0;

	}
	catch (const std::exception& e) {
		std::cerr << "=== [Subscriber] Exception: " << e.what() << std::endl;
		return EXIT_FAILURE;
	}
}