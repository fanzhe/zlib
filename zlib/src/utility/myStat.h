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
  double cm_cnt_prune;
  double cr_cnt;
  double cm_cnt;
  double cr_cnt_predict;

  double org_cr_v;
  double org_cr_e;
  double red_cr_v;
  double red_cr_e;
  double nec_effect_v;
  double nec_effect_e;
  double nc_effect_v;
  double nc_effect_e;
  double org_psb_map_cnt;
  double red_psb_map_cnt;

  double cr_time;
  double cr_bfs_time;
  double cr_cont_time;
  double cr_cont_time_1;
  double cr_cont_time_2;
  double nec_time;
  double nc_time;
  double cr_predict_time;
  double cm_time;
  double cache_size;
  double match_time;
  double canon_cm_time;
  double decomp_cm_time;
  double each_tt_time;

  // for each invokes of query
  double isSubgraphOf2e_time;
  double mul_add_time;

  // for total
  double tt;
  double answer_cnt;
  double avg_cm_cnt;
  double avg_cr_cnt;
  double avg_cm_cnt_prune;
  double avg_cr_cnt_predict;

  double avg_org_cr_v;
  double avg_org_cr_e;
  double avg_red_cr_v;
  double avg_red_cr_e;
  double avg_nec_effect_v;
  double avg_nec_effect_e;
  double avg_nc_effect_v;
  double avg_nc_effect_e;
  double avg_org_psb_map_cnt;
  double avg_red_psb_map_cnt;

  double avg_total_time;
  double avg_sp_time;
  double avg_cr_time;
  double avg_cr_bfs_time;
  double avg_cr_cont_time;
  double avg_cr_predict_time;
  double avg_nec_time;
  double avg_nc_time;
  double avg_cm_time;
  double avg_canon_cm_time;
  double avg_decomp_cm_time;
  double avg_match_time;
  double avg_isSubgraphOf2e_time;
  double avg_mul_add_time;
  double avg_client_time;
  double avg_encrypt_time;
  double avg_decrypt_time;
  double avg_client_msg_size;
  double avg_cache_size;

  // for encryption
  double encrypt_time;

  // for decryption
  double decrypt_time;
  double encypted_msg_cnt;

  STAT() {
    cache_size = avg_cache_size = 0;
    avg_encrypt_time = avg_decrypt_time = 0;
    cr_cont_time = cr_cont_time_1 = cr_cont_time_2 = cr_bfs_time = avg_cr_bfs_time = avg_cr_cont_time = 0;
    cr_predict_time = avg_cr_predict_time = 0;
    nec_time = nc_time = 0;
    avg_nec_time = avg_nc_time = 0;
    org_psb_map_cnt = red_psb_map_cnt = avg_org_psb_map_cnt =
        avg_red_psb_map_cnt = 0;
    org_cr_v = org_cr_e = red_cr_v = red_cr_e = avg_org_cr_v = avg_org_cr_e =
        avg_red_cr_v = avg_red_cr_e = 0;
    avg_nc_effect_e = avg_nc_effect_v = avg_nec_effect_e = avg_nec_effect_v = 0;
    nec_effect_v = nec_effect_e = nc_effect_e = nc_effect_v = cm_cnt_prune =
        cr_cnt = cm_cnt = 0;
    tt = answer_cnt = each_tt_time = cm_time = cr_time = match_time =
        decomp_cm_time = canon_cm_time = cr_cnt_predict = 0;
    avg_total_time = avg_sp_time = avg_client_time = 0;
    decrypt_time = 0;
    encypted_msg_cnt = 0;
    avg_client_msg_size = 0;
    encrypt_time = 0;
    isSubgraphOf2e_time = mul_add_time = 0;
    avg_cm_cnt = avg_cm_cnt_prune = avg_cr_cnt = avg_cr_cnt_predict = 0;
    avg_cr_time = avg_cm_time = avg_canon_cm_time = avg_decomp_cm_time =
        avg_match_time = avg_isSubgraphOf2e_time = avg_mul_add_time = 0;
  }

  void reset() {
    cache_size = avg_cache_size = 0;
    avg_encrypt_time = avg_decrypt_time = 0;
    cr_cont_time = cr_cont_time_1 = cr_cont_time_2 = cr_bfs_time = avg_cr_bfs_time = avg_cr_cont_time = 0;
    cr_predict_time = avg_cr_predict_time = 0;
    nec_time = nc_time = 0;
    avg_nec_time = avg_nc_time = 0;
    org_psb_map_cnt = red_psb_map_cnt = avg_org_psb_map_cnt =
        avg_red_psb_map_cnt = 0;
    org_cr_v = org_cr_e = red_cr_v = red_cr_e = avg_org_cr_v = avg_org_cr_e =
        avg_red_cr_v = avg_red_cr_e = 0;
    avg_nc_effect_e = avg_nc_effect_v = avg_nec_effect_e = avg_nec_effect_v = 0;
    nec_effect_v = nec_effect_e = nc_effect_e = nc_effect_v = cm_cnt_prune =
        cr_cnt = cm_cnt = 0;
    tt = answer_cnt = each_tt_time = cm_time = cr_time = match_time =
        decomp_cm_time = canon_cm_time = cr_cnt_predict = 0;
    avg_total_time = avg_sp_time = avg_client_time = 0;
    decrypt_time = 0;
    encypted_msg_cnt = 0;
    avg_client_msg_size = 0;
    encrypt_time = 0;
    isSubgraphOf2e_time = mul_add_time = 0;
    avg_cm_cnt = avg_cm_cnt_prune = avg_cr_cnt = avg_cr_cnt_predict = 0;
    avg_cr_time = avg_cm_time = avg_canon_cm_time = avg_decomp_cm_time =
        avg_match_time = avg_isSubgraphOf2e_time = avg_mul_add_time = 0;
  }

};

#endif /* MYSTAT_H_ */
