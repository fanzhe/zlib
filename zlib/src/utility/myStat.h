/*
 * myStat.h
 *
 *  Created on: Apr 29, 2014
 *      Author: zfan
 */

#ifndef MYSTAT_H_
#define MYSTAT_H_

class STAT {
 public:
  // for each invokes of spsubiso
  double cr_time;
  double cm_time;
  double match_time;
  double enum_cm_time;
  double decomp_cm_time;

  // for each invokes of query
  double isSubgraphOf2e_time;
  double mul_add_time;

  // for total
  double avg_sp_time;
  double avg_client_time;
  double avg_client_msg_size;

  // for encryption
  double encrypt_time;

  // for decryption
  double decrypt_time;
  double encypted_msg_cnt;

  STAT() {
    cm_time = cr_time = match_time = decomp_cm_time = enum_cm_time = 0;
    avg_sp_time = avg_client_time = 0;
    decrypt_time = 0;
    encypted_msg_cnt = 0;
    avg_client_msg_size = 0;
    encrypt_time = 0;
    isSubgraphOf2e_time = mul_add_time = 0;
  }

  void reset() {
    cm_time = cr_time = match_time = decomp_cm_time = enum_cm_time = 0;
    avg_sp_time = avg_client_time = 0;
    decrypt_time = 0;
    encypted_msg_cnt = 0;
    avg_client_msg_size = 0;
    encrypt_time = 0;
    isSubgraphOf2e_time = mul_add_time = 0;
  }

};

#endif /* MYSTAT_H_ */
