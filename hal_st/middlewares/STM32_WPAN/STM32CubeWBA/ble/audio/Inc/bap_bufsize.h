/*****************************************************************************
 * @file    bap_bufsize.h
 * @author  MDG
 * @brief   Definition of Basic Audio Profile GATT resource and stack buffers size
 *****************************************************************************
 * @attention
 *
 * Copyright (c) 2018-2022 STMicroelectronics.
 * All rights reserved.
 *
 * This software is licensed under terms that can be found in the LICENSE file
 * in the root directory of this software component.
 * If no LICENSE file comes with this software, it is provided AS-IS.
 *
 *****************************************************************************
 */

#ifndef BAP_BUFSIZE_H__
#define BAP_BUFSIZE_H__

/* #############################################################################
   #       Defines and MACRO used to allocate GATT Services, GATT Attributes   #
   #       and storage area for Attribute values for Basic Audio Profile       #
   #       in Server role                                                      #
   ############################################################################# */

/*
 * BAP_PACS_SRV_NUM_GATT_SERVICES : number of GATT services required for Published Audio Capabilities Service
 * registration during BAP initialization in Unicast Server and:or Broadcast Sink role.
 */
#define BAP_PACS_SRV_NUM_GATT_SERVICES                  (1u)

/*
 * BAP_ASCS_SRV_NUM_GATT_SERVICES : number of GATT services required for Audio Stream Control Service
 * registration during BAP initialization in Unicast Server role.
 */
#define BAP_ASCS_SRV_NUM_GATT_SERVICES                  (1u)

/*
 * BAP_BASS_SRV_NUM_GATT_SERVICES : number of GATT services required for Broadcast Audio Scan Service
 * registration during BAP initialization in Scan Delegator role.
 */
#define BAP_BASS_SRV_NUM_GATT_SERVICES                  (1u)

/*
 * BAP_PACS_SRV_GATT_ATTRIBUTES: this macro returns the number of GATT attributes required for Published
 * Audio Capabilities Service registration during BAP initialization in Unicast Server and/or Broadcast Sink role.
 * This number depends on the number of supported Sink PAC records and the number of supported Source PAC records.
 *
 * @param max_num_snk_pac_records: Maximum number of Sink PAC records
 *
 * @param max_num_src_pac_records: Maximum number of Source PAC records
 */
#define BAP_PACS_SRV_GATT_ATTRIBUTES(max_num_snk_pac_records,max_num_src_pac_records) \
                                                      ( 3u * (2u + (max_num_snk_pac_records+max_num_src_pac_records) \
                                                       + (((max_num_snk_pac_records) > (0x00)) ? (1) : (0)) \
                                                       + (((max_num_src_pac_records) > (0x00)) ? (1) : (0))))

/*
 * BAP_ASCS_SRV_GATT_ATTRIBUTES: this macro returns the number of GATT attributes required for Audio Stream
 * Control Service registration during BAP initialization in Unicast Server role.
 * This number depends on the number of supported Sink Audio Stream Endpoints and the number of supported
 * Source Audio Stream Endpoints.
 *
 * @param max_num_snk_ase: Maximum number of Sink Audio Stream Endpoints
 *
 * @param max_num_src_ase: Maximum number of Source Audio Stream Endpoints
 */
#define BAP_ASCS_SRV_GATT_ATTRIBUTES(max_num_snk_ase,max_num_src_ase) \
                                                        (3u *(1u + (max_num_snk_ase + max_num_src_ase)))

/*
 * BAP_BASS_SRV_GATT_ATTRIBUTES: this macro returns the number of GATT attributes required for Broadcast Audio Scan
 * Service registration during BAP initialization in Scan Delegator role.
 * This number depends on the number of supported Broadcast Sources.
 *
 * @param max_num_bsrc: Maximum number of Broadcast Sources.
 *
 */
#define BAP_BASS_SRV_GATT_ATTRIBUTES(max_num_bsrc)      (3u *( max_num_bsrc + 1u))

/*
 * BAP_PAC_RECORD_ATT_VALUE_ARRAY_SIZE: this macro returns the size of the storage area for Attribute values
 * of a PAC Record in PACS Server.
 *
 * @param num_ble_links: Maximum number of simultaneous connections that the device will support.
 *
 * @param max_codec_specific_capabilities_length: length of codec specific capablitities block the PAC Record
 *
 * @param max_medata_length: maximum supported length of the metadata associated to the PAC Record
 */
#define BAP_PAC_RECORD_ATT_VALUE_ARRAY_SIZE(num_ble_links,max_codec_specific_capabilities_length,max_medata_length) \
                                (13u+(2u*num_ble_links)+max_codec_specific_capabilities_length+max_medata_length)

/*
 * BAP_PACS_SRV_ATT_VALUE_ARRAY_SIZE: this macro returns the size of the storage area for Attribute values
 * in PACS Server.
 *
 * @param num_ble_links: Maximum number of simultaneous connections that the device will support.
 *
 * @param max_num_snk_pac_records: Maximum number of Sink PAC records
 *
 * @param max_num_src_pac_records: Maximum number of Source PAC records
 *
 * @param total_pac_records_att_array_size: total size of the storage area for Attributes Values
 *                                           of the PAC records to register.
 */
#define BAP_PACS_SRV_ATT_VALUE_ARRAY_SIZE(num_ble_links,max_num_snk_pac_records,max_num_src_pac_records,total_pac_records_att_array_size) \
                                ((18u+(4u*num_ble_links)) \
                                + (((max_num_snk_pac_records) > (0x00)) ? (9u+2*num_ble_links) : (0)) \
                                + (((max_num_src_pac_records) > (0x00)) ? (9u+2*num_ble_links) : (0)) \
                                + total_pac_records_att_array_size)



/*
 * BAP_ASE_ATT_VALUE_ARRAY_SIZE: this macro returns the size of the storage area for Attribute values
 * of an Audio Stream Endpoint in ASCS Server.
 *
 * @param num_ble_links: Maximum number of simultaneous connections that the device will support.
 *
 * @param max_codec_config_length: Maximum Codec configuration length of the Audio Stream Endpoint
 *
 * @param max_metadata_length: Maximum Metadata length of the Audio Stream Endpoint
 */
#define BAP_ASE_ATT_VALUE_ARRAY_SIZE(num_ble_links,max_codec_config_length,max_metadata_length) \
                                ((2u*num_ble_links)+MAX((30u + max_codec_config_length),(10u + max_metadata_length)))

/*
 * BAP_ASCS_SRV_ATT_VALUE_ARRAY_SIZE: this macro returns the size of the storage area for Attribute values
 * in ASCS Server.
 *
 * @param num_ble_links: Maximum number of simultaneous connections that the device will support.
 *
 * @param total_ases_att_array_size: total size of the storage area for Attributes Values
 *                                    of the Audio Stream Endpoints to register.
 */
#define BAP_ASCS_SRV_ATT_VALUE_ARRAY_SIZE(num_ble_links,total_ases_att_array_size) \
                                ((260u+(2u*num_ble_links)) + total_ases_att_array_size)

/*
 * BAP_BASS_SRV_ATT_VALUE_ARRAY_SIZE: this macro returns the size of the storage area for Attribute values
 * in BASS Server.
 *
 * @param max_num_bsrc: Maximum number of Broadcast Sources.
 *
 * @param num_ble_links: Maximum number of simultaneous connections that the device will support.
 *
 * @param max_metadata_length_per_bsrc: Maximum Metadata length per Broadcast Source
 */
#define BAP_BASS_SRV_ATT_VALUE_ARRAY_SIZE(max_num_bsrc_info,num_ble_links,max_metadata_length_per_bsrc) \
                                (26u + max_metadata_length_per_bsrc + \
                                 (max_num_bsrc_info * (41u+(2u*num_ble_links)+max_metadata_length_per_bsrc)))



/* #############################################################################
   #       Defines and MACRO used to allocate memory resource of               #
   #       Basic Audio Profile in BAP_Config_t                                 #
   #      (BAP_ASCS_CLT_TOTAL_BUFFER_SIZE , BAP_ASCS_SRV_TOTAL_BUFFER_SIZE)    #
   #      (BAP_ASCS_CLT_TOTAL_BUFFER_SIZE , BAP_ASCS_SRV_TOTAL_BUFFER_SIZE)    #
   #      (BAP_PACS_CLT_TOTAL_BUFFER_SIZE , BAP_PACS_SRV_TOTAL_BUFFER_SIZE)    #
   #      (BAP_BASS_CLT_TOTAL_BUFFER_SIZE , BAP_BASS_SRV_TOTAL_BUFFER_SIZE)    #
   ############################################################################# */

/*
 * BAP_SRV_MEM_PER_PAC_RECORD_SIZE_BYTES: memory size used per PAC Record in Unicast Server role or Broadcast Sink role
 */
#define BAP_SRV_MEM_PER_PAC_RECORD_SIZE_BYTES           (4u)

/*
 * BAP_SRV_GATT_CHAR_MEM_PER_BSRC_INFO_SIZE_BYTES: memory size used for GATT characteristic per Broadcast Source
 *                                                 Information in Scan Delegator role
 */
#define BAP_SRV_GATT_CHAR_MEM_PER_BSRC_INFO_SIZE_BYTES  (4u)

/*
 * BAP_MEM_PER_ATT_HANDLE_SIZE_BYTES: memory size used per Attribute Handle (2-Bytes) in Server Database
 */
#define BAP_MEM_PER_ATT_HANDLE_SIZE_BYTES               (2u)

/*
 * BAP_MEM_PER_ASE_SIZE_BYTES: memory size used per Audio Stream Endpoint
 */
#define BAP_MEM_PER_ASE_SIZE_BYTES                      (96u)

/*
 * BAP_ASCS_SRV_INST_MEM_PER_CONN_SIZE_BYTES: memory size used to allocate ASCS Server Context
 *                                            per Unicast Server Connection Instance
 */
#define BAP_ASCS_SRV_INST_MEM_PER_CONN_SIZE_BYTES       (20u)

/*
 * BAP_ASCS_SRV_MEM_CONTEXT_SIZE_BYTES: memory size used by Unicast Server for ACSC Server Context Allocation
 */
#define BAP_ASCS_SRV_MEM_CONTEXT_SIZE_BYTES             (60u)

/*
 * BAP_MEM_PER_ATT_CHAR_SIZE_BYTES: memory size used per Characteristic in GATT Client side
 */
#define BAP_MEM_PER_ATT_CHAR_SIZE_BYTES                 (16u)

/*
 * BAP_ASCS_CLT_INST_MEM_PER_CONN_SIZE_BYTES: memory size used to allocate ASCS Client Context
 *                                            per Unicast Client Connection Instance
 */
#define BAP_ASCS_CLT_INST_MEM_PER_CONN_SIZE_BYTES       364u

/*
 * BAP_ASCS_CLT_MEM_CONTEXT_SIZE_BYTES: memory size used per Unicast Client for ACSC Client Context Allocation
 */
#define BAP_ASCS_CLT_MEM_CONTEXT_SIZE_BYTES             (32u)

/*
 * BAP_MEM_PER_CIG_SIZE_BYTES: memory size used per CIG
 */
#define BAP_MEM_PER_CIG_SIZE_BYTES                      (36u)
/*
 * BAP_MEM_PER_CIS_SIZE_BYTES: memory size used per CIS
 */
#define BAP_MEM_PER_CIS_SIZE_BYTES                      (24u)

/*
 * BAP_PACS_CLT_INST_MEM_PER_CONN_SIZE_BYTES: memory size used per Connection PACS Client Instance by Unicast Client
 *                                            and Broadcast Assistant
 */
#define BAP_PACS_CLT_INST_MEM_PER_CONN_SIZE_BYTES       (92u)

/*
 * BAP_PACS_CLT_MEM_CONTEXT_SIZE_BYTES: memory size used for PACS Client Context Allocation
 *                                      by Unicast Client and Broadcast Assistant
 */
#define BAP_PACS_CLT_MEM_CONTEXT_SIZE_BYTES             (28u)

/*
 * BAP_NVM_INST_PER_CONN_SIZE_BYTES: memory size used for Non-Volatile Memory Management and BAP Services restoration
 *                                   by Unicast Client, Unicast Server, Scan Delegator and Broadcast Assistant
 */
#define BAP_NVM_MGMT_PER_CONN_SIZE_BYTES                (8u)

/*
 * BAP_BASS_CLT_INST_MEM_PER_CONN_SIZE_BYTES: memory size used per Connection BASS Client Instance
 *                                            by Broadcast Assistant
 */
#define BAP_BASS_CLT_INST_MEM_PER_CONN_SIZE_BYTES       (52u)

/*
 * BAP_BASS_CLT_MEM_CONTEXT_SIZE_BYTES: memory size used for BASS Client Context Allocation by Broadcast Assistant
 */
#define BAP_BASS_CLT_MEM_CONTEXT_SIZE_BYTES             (16u)


/*
 * BAP_BASS_SERV_MEM_CONTEXT_SIZE_BYTES: memory size used for BIS Info storage per BIS
 *                                      (size of the structure BAP_BASE_BIS_t)
 */
#define BAP_BIS_INFO_MEM_PER_BIS_SIZE_BYTES             (8u)

/*
 * BAP_MEM_PER_BASE_SUBGROUP_SIZE_BYTES: memory size used for BASE subgroup allocation in  Scan Delegator
 */
#define BAP_MEM_PER_BASE_SUBGROUP_SIZE_BYTES            (16u)

/*
 * BAP_MEM_PER_BSRC_STATE_SIZE_BYTES: memory size used for Broadcast Receive State Information allocation
 *                                   in Scan Delegator
 */
#define BAP_MEM_PER_BSRC_STATE_SIZE_BYTES               (136u)


/*
 * BAP_BASS_SERV_MEM_CONTEXT_SIZE_BYTES: memory size used for BASS Server Context Allocation by Scan Delegator
 */
#define BAP_BASS_SRV_MEM_CONTEXT_SIZE_BYTES             (320u)

/*
 * BAP_MEM_BLOCKS_ASE_PER_CONN_SIZE_BYTES: this macro returns the amount of memory, in bytes, needed for the storage
 * of the Audio Stream Endpoint structures in Unicast Client and Unicast Server whose size depends on the number of
 * supported Sink Audio Stream Endpoints, the number of supported Source Audio Stream Endpoints, the maximum
 * supported Codec Configuration length and the maximum supported Metadata Length.
 *
 * @param max_num_snk_ase_per_link: Maximum number of Sink Audio Stream Endpoints per connection
 *
 * @param max_num_src_ase_per_link: Maximum number of Source Audio Stream Endpoints per connection
 *
 * @param max_codec_config_length_per_ase: Maximum Codec configuration length per Audio Stream Endpoint
 *
 * @param max_metadata_length_per_ase: Maximum Metadata length per Audio Stream Endpoint
 */
#define BAP_MEM_BLOCKS_ASE_PER_CONN_SIZE_BYTES(max_num_snk_ase_per_link,max_num_src_ase_per_link,\
                                               max_codec_config_length_per_ase,max_metadata_length_per_ase) \
          (DIVC(((BAP_MEM_PER_ASE_SIZE_BYTES + max_codec_config_length_per_ase + max_metadata_length_per_ase) \
            *(max_num_snk_ase_per_link+max_num_src_ase_per_link)),4u) * 4u)


/*
 * BAP_MEM_BLOCKS_CIG_SIZE_BYTES: this macro returns the amount of memory, in bytes, needed for the storage of the
 * Isocrhronous Channel information whose size depends on the number of supported CIGes and CISes per CIG.
 *
 * @param max_num_cig: Maximum number of CIGes
 *
 * @param max_num_cis_per_cig: Maximum number of CISes per CIG
 */
#define BAP_MEM_BLOCKS_CIG_SIZE_BYTES(max_num_cig,max_num_cis_per_cig) \
          ((BAP_MEM_PER_CIG_SIZE_BYTES + (BAP_MEM_PER_CIS_SIZE_BYTES * max_num_cis_per_cig))*max_num_cig)

/*
 * BAP_BASS_SRV_INST_MEM_PER_BSRC_INFO_SIZE_BYTES: this macro returns the amount of memory, in bytes, needed for the
 * storage of a Broadcast Source Information Instance structures in Scan Delegator whose size depends
 * on the number of BISes per Broadcast Source,the maximum supported Codec Configuration length, the maximum supported
 * Metadata Length and the maximum number of number of subgroups present in the BASE structure, used to describe a BIG.
 *
 * @param max_num_bis_per_bsrc: Maximum number of BISes per Broadcast Source
 *
 * @param max_codec_config_length_per_bsrc: Maximum Codec configuration length per Broadcast Source
 *
 * @param max_metadata_length_per_bsrc: Maximum Metadata length per Broadcast Source
 *
 * @param max_num_base_subgroups: maximum number of number of subgroups present in the BASE structure,
 *                               used to describe a BIG.
 */
#define BAP_BASS_SRV_INST_MEM_PER_BSRC_INFO_SIZE_BYTES(max_num_bis_per_bsrc,max_codec_config_length_per_bsrc,\
                                                       max_metadata_length_per_bsrc,max_num_base_subgroups) \
        (BAP_SRV_GATT_CHAR_MEM_PER_BSRC_INFO_SIZE_BYTES + (BAP_BIS_INFO_MEM_PER_BIS_SIZE_BYTES * max_num_bis_per_bsrc) \
        + ( BAP_MEM_PER_BSRC_STATE_SIZE_BYTES) \
        + (DIVC(((((2u + 4u) * max_num_bis_per_bsrc) + max_codec_config_length_per_bsrc + max_metadata_length_per_bsrc + (max_num_bis_per_bsrc * max_codec_config_length_per_bsrc))),4u)*4u) \
        + (DIVC(((max_metadata_length_per_bsrc + BAP_MEM_PER_BASE_SUBGROUP_SIZE_BYTES) * max_num_base_subgroups),4)*4u))


/*
 * BAP_ASCS_SRV_TOTAL_BUFFER_SIZE: this macro returns the amount of memory, in bytes, needed for the storage of the
 * Audio Stream Control Server Context whose size depends on the number of supported Sink Audio Stream Endpoints,
 * the number of supported Source Audio Stream Endpoints, the size required to support the Sink and Source Audio
 * Stream Endpoints per connection and the number of supported connections.
 *
 * @param num_ble_links: Maximum number of BLE connections
 *
 * @param max_num_snk_ase_per_link: Maximum number of Sink Audio Stream Endpoints per connection
 *
 * @param max_num_src_ase_per_link: Maximum number of Source Audio Stream Endpoints per connection
 *
 * @param mem_block_ase_size_per_link: size required to support the Sink and Source Audio Stream Endpoints
 *                                   per connection (calculated thanks to BAP_MEM_BLOCKS_ASE_PER_CONN_SIZE_BYTES macro)
 *
 */
#define BAP_ASCS_SRV_TOTAL_BUFFER_SIZE(num_ble_links,max_num_snk_ase_per_link,\
                                        max_num_src_ase_per_link,mem_block_ase_size_per_link) \
        (BAP_ASCS_SRV_MEM_CONTEXT_SIZE_BYTES + (mem_block_ase_size_per_link * num_ble_links) \
        + (DIVC(((max_num_snk_ase_per_link+max_num_src_ase_per_link) * BAP_MEM_PER_ATT_HANDLE_SIZE_BYTES),4u) * 4u) \
        + (BAP_ASCS_SRV_INST_MEM_PER_CONN_SIZE_BYTES * num_ble_links))

/*
 * BAP_ASCS_CLT_TOTAL_BUFFER_SIZE: this macro returns the amount of memory, in bytes, needed for the storage of the
 * Audio Stream Control Client Context whose size depends on the number of supported Sink Audio Stream Endpoints,
 * the number of supported Source Audio Stream Endpoints, the size required to support the Sink and Source Audio
 * Stream Endpoints per connection and the number of supported Unicast Links.
 *
 * @param num_ucl_links: Maximum number of Unicast Links
 *
 * @param max_num_snk_ase_per_link: Maximum number of Sink Audio Stream Endpoints per connection
 *
 * @param max_num_src_ase_per_link: Maximum number of Source Audio Stream Endpoints per connection
 *
 * @param mem_block_ase_size_per_link: size required to support the Sink and Source Audio Stream Endpoints per
 *                                    connection (calculated thanks to BAP_MEM_BLOCKS_ASE_PER_CONN_SIZE_BYTES macro)
 *
 */
#define BAP_ASCS_CLT_TOTAL_BUFFER_SIZE(num_ucl_links,max_num_snk_ase_per_link, \
                                       max_num_src_ase_per_link,mem_block_ase_size_per_link) \
        (BAP_ASCS_CLT_MEM_CONTEXT_SIZE_BYTES + (mem_block_ase_size_per_link * num_ucl_links) \
        + ((max_num_snk_ase_per_link+max_num_src_ase_per_link) * num_ucl_links * BAP_MEM_PER_ATT_CHAR_SIZE_BYTES) \
        + (DIVC(((BAP_ASCS_CLT_INST_MEM_PER_CONN_SIZE_BYTES + (max_num_snk_ase_per_link+max_num_src_ase_per_link)) * num_ucl_links),4u) * 4u))


/*
 * BAP_PACS_SRV_TOTAL_BUFFER_SIZE: this macro returns the amount of memory, in bytes, needed for the storage of the
 * Published Audio Capabilities Server Context whose size depends on the number of supported Sink PAC records and
 * the number of supported Source PAC records.
 *
 * @param max_num_snk_pac_records: Maximum number of Sink PAC records
 *
 * @param max_num_src_pac_records: Maximum number of Source PAC records
 */
#define BAP_PACS_SRV_TOTAL_BUFFER_SIZE(max_num_snk_pac_records,max_num_src_pac_records) \
          (DIVC(((max_num_snk_pac_records+max_num_src_pac_records) * BAP_SRV_MEM_PER_PAC_RECORD_SIZE_BYTES),4u) * 4u)

/*
 * BAP_PACS_CLT_TOTAL_BUFFER_SIZE: this macro returns the amount of memory, in bytes, needed for the storage of the
 * Published Audio Capabilities Client Context whose size depends on the number of supported Unicast Links, the
 * number of Broadcast Assistant Links, the number of supported PAC Records per connection, and the maximum number of
 * supported ble connections.
 *
 * @param num_ble_links: Maximum number of supported Ble connection
 *
 * @param num_ucl_links: Maximum number of Unicast Links
 *
 * @param num_ba_links: Maximum number of Broadcast Assistant Links
 *
 * @param num_pac_records_per_link: Maximum number of PAC Records per connection
 */
#define BAP_PACS_CLT_TOTAL_BUFFER_SIZE(num_ble_links,num_ucl_links,num_ba_links,num_pac_records_per_link) \
              (BAP_PACS_CLT_MEM_CONTEXT_SIZE_BYTES \
              + (BAP_MEM_PER_ATT_CHAR_SIZE_BYTES * (MIN((num_ucl_links + num_ba_links),num_ble_links) *num_pac_records_per_link)) \
              + (BAP_PACS_CLT_INST_MEM_PER_CONN_SIZE_BYTES * MIN((num_ucl_links + num_ba_links),num_ble_links)))

/*
 * BAP_NVM_MGMT_TOTAL_BUFFER_SIZE: this macro returns the amount of memory, in bytes, needed for the storage of the
 * BAP Services restoration whose size depends on the number of supported connections.
 *
 * @param num_ble_links: Maximum number of connections
 */
#define BAP_NVM_MGMT_TOTAL_BUFFER_SIZE(num_ble_links) \
              (BAP_NVM_MGMT_PER_CONN_SIZE_BYTES * num_ble_links)


/*
 * BAP_BASS_CLT_TOTAL_BUFFER_SIZE: this macro returns the amount of memory, in bytes, needed for the storage of the
 * Broadcast Audio Scan Service Client Context whose size depends on the number of supported Broadcast Assistant links,
 * and the number of Broadcast Source Information contexts per connection.
 *
 * @param num_ba_links: Maximum number of Broadcast Assistant Links
 *
 * @param max_num_bsrc_info_per_link: Maximum number of Broadcast Source Information contexts per connection
 *                                    with remote Scan Delegator
 *
 */
#define BAP_BASS_CLT_TOTAL_BUFFER_SIZE(num_ba_links,max_num_bsrc_info_per_link) \
        (BAP_BASS_CLT_MEM_CONTEXT_SIZE_BYTES \
        + (BAP_MEM_PER_ATT_CHAR_SIZE_BYTES * (max_num_bsrc_info_per_link * num_ba_links)) \
        + (BAP_BASS_CLT_INST_MEM_PER_CONN_SIZE_BYTES * num_ba_links))


/*
 * BAP_BASS_SRV_TOTAL_BUFFER_SIZE: this macro returns the amount of memory, in bytes, needed for the storage of the
 * Broadcast Source information by the Scan Delegator whose size depends on the Maximum number of Broadcast Source
 * Information, the number of BISes per Broadcast Source,the maximum supported Codec Configuration length, the maximum
 * supported Metadata Length and the maximum number of number of subgroups present in the BASE structure, used to
 * describe a BIG.
 *
 * @param max_num_bsrc_info: Maximum number of Broadcast Source Information to handle
 *
 * @param max_num_bis_per_bsrc: Maximum number of BISes per Broadcast Source
 *
 * @param max_codec_config_length_per_bsrc: Maximum Codec configuration length per Broadcast Source
 *
 * @param max_metadata_length_per_ase: Maximum Metadata length per Broadcast Source
 *
 * @param max_num_base_subgroups: maximum number of number of subgroups present in the BASE structure,
 *                               used to describe a BIG.
 *
 */
#define BAP_BASS_SRV_TOTAL_BUFFER_SIZE(max_num_bsrc_info,max_num_bis_per_bsrc,\
                                      max_codec_config_length_per_bsrc,max_metadata_length_per_bsrc, \
                                      max_num_base_subgroups) \
        (BAP_BASS_SRV_MEM_CONTEXT_SIZE_BYTES \
          + (max_num_bsrc_info * BAP_BASS_SRV_INST_MEM_PER_BSRC_INFO_SIZE_BYTES(max_num_bis_per_bsrc,\
                                                                                max_codec_config_length_per_bsrc, \
                                                                                max_metadata_length_per_bsrc, \
                                                                                max_num_base_subgroups)))


#endif /* BAP_BUFSIZE_H__ */
