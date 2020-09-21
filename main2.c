#include <stdio.h>
#include <string.h>
#include <arpa/inet.h>

#include <stdio.h>

#include <stdlib.h>
#include "cJSON.h"

//#include "memory_pool.h"
#include "memory_pool_example.h"


const char* json_msg = "{\"Event-Name\":\"CHANNEL_CREATE\",\"Core-UUID\":\"63cd5e4d-c1ab-4d1c-b316-e61f5ac4ff3d\",\"FreeSWITCH-Hostname\":\"zj-4005\",\"FreeSWITCH-Switchname\":\"zj-4005\",\"FreeSWITCH-IPv4\":\"10.0.40.203\",\"FreeSWITCH-IPv6\":\"::1\",\"Event-Date-Local\":\"2020-09-18 17:43:54\",\"Event-Date-GMT\":\"Fri, 18 Sep 2020 09:43:54 GMT\",\"Event-Date-Timestamp\":\"1600422234412424\",\"Event-Calling-File\":\"switch_core_state_machine.c\",\"Event-Calling-Function\":\"switch_core_session_run\",\"Event-Calling-Line-Number\":\"629\",\"Event-Sequence\":\"4340\",\"Channel-State\":\"CS_INIT\",\"Channel-Call-State\":\"DOWN\",\"Channel-State-Number\":\"2\",\"Channel-Name\":\"sofia/internal/1000@10.0.40.203:18652\",\"Unique-ID\":\"45fbdb63-aaf6-43ce-a04b-0d69dd7a7d4a\",\"Call-Direction\":\"outbound\",\"Presence-Call-Direction\":\"outbound\",\"Channel-HIT-Dialplan\":\"false\",\"Channel-Presence-ID\":\"1000@10.0.40.203\",\"Channel-Call-UUID\":\"45fbdb63-aaf6-43ce-a04b-0d69dd7a7d4a\",\"Answer-State\":\"ringing\",\"Caller-Direction\":\"outbound\",\"Caller-Logical-Direction\":\"outbound\",\"Caller-Caller-ID-Number\":\"0000000000\",\"Caller-Orig-Caller-ID-Number\":\"0000000000\",\"Caller-Callee-ID-Name\":\"Outbound Call\",\"Caller-Callee-ID-Number\":\"1000\",\"Caller-ANI\":\"0000000000\",\"Caller-Destination-Number\":\"1000\",\"Caller-Unique-ID\":\"45fbdb63-aaf6-43ce-a04b-0d69dd7a7d4a\",\"Caller-Source\":\"c:\\\\bamboo-home\\\\xml-data\\\\build-dir\\\\fs-fwr-job1\\\\src\\\\switch_ivr_originate.c\",\"Caller-Context\":\"default\",\"Caller-Channel-Name\":\"sofia/internal/1000@10.0.40.203:18652\",\"Caller-Profile-Index\":\"1\",\"Caller-Profile-Created-Time\":\"1600422234412424\",\"Caller-Channel-Created-Time\":\"1600422234412424\",\"Caller-Channel-Answered-Time\":\"0\",\"Caller-Channel-Progress-Time\":\"0\",\"Caller-Channel-Progress-Media-Time\":\"0\",\"Caller-Channel-Hangup-Time\":\"0\",\"Caller-Channel-Transfer-Time\":\"0\",\"Caller-Channel-Resurrect-Time\":\"0\",\"Caller-Channel-Bridged-Time\":\"0\",\"Caller-Channel-Last-Hold\":\"0\",\"Caller-Channel-Hold-Accum\":\"0\",\"Caller-Screen-Bit\":\"true\",\"Caller-Privacy-Hide-Name\":\"false\",\"Caller-Privacy-Hide-Number\":\"false\",\"variable_direction\":\"outbound\",\"variable_is_outbound\":\"true\",\"variable_uuid\":\"45fbdb63-aaf6-43ce-a04b-0d69dd7a7d4a\",\"variable_call_uuid\":\"45fbdb63-aaf6-43ce-a04b-0d69dd7a7d4a\",\"variable_session_id\":\"39\",\"variable_sip_local_network_addr\":\"10.0.40.203\",\"variable_sip_profile_name\":\"internal\",\"variable_video_media_flow\":\"disabled\",\"variable_text_media_flow\":\"disabled\",\"variable_channel_name\":\"sofia/internal/1000@10.0.40.203:18652\",\"variable_sip_destination_url\":\"sip:1000@10.0.40.203:18652;rinstance=397a3eff378dd913\",\"variable_dialed_user\":\"1000\",\"variable_dialed_domain\":\"10.0.40.203\",\"variable_sip_invite_domain\":\"10.0.40.203\",\"variable_presence_id\":\"1000@10.0.40.203\",\"variable_originate_early_media\":\"true\",\"variable_rtp_use_codec_string\":\"PCMA,PCMU\",\"variable_local_media_ip\":\"10.0.40.203\",\"variable_local_media_port\":\"28460\",\"variable_advertised_media_ip\":\"10.0.40.203\",\"variable_audio_media_flow\":\"sendrecv\",\"variable_rtp_local_sdp_str\":\"v=0\\r\\no=FreeSWITCH 1600393774 1600393775 IN IP4 10.0.40.203\\r\\ns=FreeSWITCH\\r\\nc=IN IP4 10.0.40.203\\r\\nt=0 0\\r\\nm=audio 28460 RTP/AVP 8 0 101\\r\\na=rtpmap:8 PCMA/8000\\r\\na=rtpmap:0 PCMU/8000\\r\\na=rtpmap:101 telephone-event/8000\\r\\na=fmtp:101 0-16\\r\\na=ptime:20\\r\\na=sendrecv\\r\\n\",\"variable_sip_outgoing_contact_uri\":\"<sip:mod_sofia@10.0.40.203:5060>\",\"variable_sip_req_uri\":\"1000@10.0.40.203:18652;rinstance=397a3eff378dd913\",\"variable_sip_to_host\":\"10.0.40.203:18652\",\"variable_sip_from_host\":\"10.0.40.203\",\"variable_sofia_profile_name\":\"internal\",\"variable_recovery_profile_name\":\"internal\",\"variable_sofia_profile_url\":\"sip:mod_sofia@10.0.40.203:5060\"}";

int main(int argc,char** argv) {
	//int ret = bit_range2(5, 12, size);
	/*int z = 0;

	memory_pool_t* mp = mp_create(8192);
	memory_node_t* p = mp_malloc(mp, 27);
	mp_free(mp, p);
	p = mp_malloc(mp, 20);
	mp_free(mp, p);
	p = mp_malloc(mp, 127);
	mp_free(mp, p);
	p = mp_malloc(mp, 60);
	mp_free(mp, p);
	p = mp_malloc(mp, 200);
	mp_free(mp, p);
	p = mp_malloc(mp, 180);
	mp_free(mp, p);
	p = mp_malloc(mp, 90);
	mp_free(mp, p);
	++z;*/
	
	
	


	if (argc < 2) {
		printf("exe 1 or n\n");
		return 0;
	}

	int type = atoi(argv[1]);
	if (type == 1) {
		eg_create_pool(10 * 1024 * 1024);
		cJSON_Hooks cjh;
		cjh.malloc_fn = eg_malloc;
		cjh.free_fn = eg_free;
		cJSON_InitHooks(&cjh);


		for (int i = 0; i < 100000; ++i) {
			cJSON* obj = cJSON_Parse(json_msg);
			cJSON_Delete(obj);
		}
		eg_destroy_pool();
	}
	else if (type == 2) {
		for (int i = 0; i < 100000; ++i) {
			cJSON* obj = cJSON_Parse(json_msg);
			cJSON_Delete(obj);
		}
	}
	return 0;
}