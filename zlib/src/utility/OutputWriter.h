/*
 * OutputWriter.h
 *
 *  Created on: Apr 30, 2014
 *      Author: zfan
 */

#ifndef OUTPUTWRITER_H_
#define OUTPUTWRITER_H_

#include "GlobalDefinition.h"
#include "myStat.h"

#include <fstream>

using namespace std;

class OutputWriter {
 public:
  char detailed_result_file_name[256];
  ofstream detailed_writer;

  char avg_result_file_name[256];
  ofstream avg_writer;

  OutputWriter() {
//    detailed_writer = avg_writer = cout;
  }

  OutputWriter(const char* detailed_name, const char* avg_name) {
    strcpy(detailed_result_file_name, detailed_name);
    strcpy(avg_result_file_name, avg_name);

    detailed_writer.open(detailed_result_file_name);
    avg_writer.open(avg_result_file_name);

    ASSERT(detailed_writer.is_open());
    ASSERT(avg_writer.is_open());
  }

  void writeDetailsubIsoResult(STAT* myStat) {
    detailed_writer << "cr_cnt: " << myStat->cr_cnt << endl;
    detailed_writer << "  cr_cnt_predict: " << myStat->cr_cnt_predict << endl;
    detailed_writer << "org_cr_v: " << myStat->org_cr_v / myStat->cr_cnt
                    << endl;
    detailed_writer << "org_cr_e: " << myStat->org_cr_e / myStat->cr_cnt
                    << endl;
    detailed_writer << "red_cr_v: " << myStat->red_cr_v / myStat->cr_cnt
                    << endl;
    detailed_writer << "red_cr_e: " << myStat->red_cr_e / myStat->cr_cnt
                    << endl;
    detailed_writer << "nec_effect: " << endl;
    detailed_writer << "  nec_effect_v: "
                    << myStat->nec_effect_v / myStat->cr_cnt << endl;
    detailed_writer << "  nec_effect_e: "
                    << myStat->nec_effect_e / myStat->cr_cnt << endl;
    detailed_writer << "nc_effect: " << endl;
    detailed_writer << "  nc_effect_v: " << myStat->nc_effect_v / myStat->cr_cnt
                    << endl;
    detailed_writer << "  nc_effect_e: " << myStat->nc_effect_e / myStat->cr_cnt
                    << endl;

    detailed_writer << "cm_cnt: " << myStat->cm_cnt << endl;
    detailed_writer << "  cm_cnt_prune: " << myStat->cm_cnt_prune << endl;
    detailed_writer << "total_time: " << myStat->each_tt_time << endl;
    detailed_writer << "  cr_time: " << myStat->cr_time << endl;
    detailed_writer << "    cr_bfs_time: " << myStat->cr_bfs_time << endl;
    detailed_writer << "    cr_cont_time: " << myStat->cr_cont_time << endl;
    detailed_writer << "    nec_time: " << myStat->nec_time << endl;
    detailed_writer << "    nc_time: " << myStat->nc_time << endl;
    detailed_writer << "    cr_predict_time: " << myStat->cr_predict_time << endl;
    detailed_writer << "  cm_time: " << myStat->cm_time << endl;
    detailed_writer << "    canon_cm_time: " << myStat->canon_cm_time << endl;
    detailed_writer << "    decomp_cm_time: " << myStat->decomp_cm_time << endl;
    detailed_writer << "    match_time: " << myStat->match_time << endl;
  }

  void writeDetailMatchResult(STAT* myStat) {
    detailed_writer << "      isSubgraphOf2e_time: "
                    << myStat->isSubgraphOf2e_time << endl;
    detailed_writer << "        mul_add_time: " << myStat->mul_add_time << endl;
    detailed_writer << "        decrypt_time: " << myStat->decrypt_time << endl;
    detailed_writer << "org_psb_map_cnt: " << myStat->org_psb_map_cnt << endl;
    detailed_writer << "red_psb_map_cnt: " << myStat->red_psb_map_cnt << endl;
    detailed_writer << endl;
  }

  void writeAvgResult(STAT* myStat) {
    avg_writer << "total: " << myStat->tt << " / " << myStat->answer_cnt
               << endl;

    double avg_cr_cnt = myStat->avg_cr_cnt / myStat->tt;
    avg_writer << "avg_cr_cnt: " << myStat->avg_cr_cnt / myStat->tt << endl;
    avg_writer << "  avg_cr_cnt_predict: "
               << myStat->avg_cr_cnt_predict / myStat->tt << endl;
    avg_writer << "avg_org_cr_v: " << myStat->avg_org_cr_v / (myStat->tt * avg_cr_cnt) << endl;
    avg_writer << "avg_org_cr_e: " << myStat->avg_org_cr_e / (myStat->tt * avg_cr_cnt) << endl;
    avg_writer << "avg_red_cr_v: " << myStat->avg_red_cr_v / (myStat->tt * avg_cr_cnt) << endl;
    avg_writer << "avg_red_cr_e: " << myStat->avg_red_cr_e / (myStat->tt * avg_cr_cnt) << endl;
    avg_writer << "avg_nec_effect:" << endl;
    avg_writer << "  avg_nec_effect_v: "
               << myStat->avg_nec_effect_v / (myStat->tt * avg_cr_cnt) << endl;
    avg_writer << "  avg_nec_effect_e: "
               << myStat->avg_nec_effect_e / (myStat->tt * avg_cr_cnt) << endl;
    avg_writer << "avg_nc_effect:" << endl;
    avg_writer << "  avg_nc_effect_v: "
               << myStat->avg_nc_effect_v / (myStat->tt * avg_cr_cnt) << endl;
    avg_writer << "  avg_nc_effect_e: "
               << myStat->avg_nc_effect_e / (myStat->tt * avg_cr_cnt) << endl;

    avg_writer << "avg_cm_cnt: " << myStat->avg_cm_cnt / myStat->tt << endl;
    avg_writer << "  avg_cm_cnt_prune: "
               << myStat->avg_cm_cnt_prune / myStat->tt << endl;

    avg_writer << "avg_total_time: " << myStat->avg_total_time / myStat->tt << endl;
    avg_writer << "  avg_sp_time: " << myStat->avg_sp_time / myStat->tt << endl;
    avg_writer << "    avg_cr_time: " << myStat->avg_cr_time / myStat->tt << endl;
    avg_writer << "      avg_cr_bfs_time: " << myStat->avg_cr_bfs_time /myStat->tt << endl;
    avg_writer << "      avg_cr_cont_time: " << myStat->avg_cr_cont_time / myStat->tt << endl;
    avg_writer << "      avg_nec_time: " << myStat->avg_nec_time / myStat->tt << endl;
    avg_writer << "      avg_nc_time: " << myStat->avg_nc_time / myStat->tt << endl;
    avg_writer << "      avg_cr_predict_time: " << myStat->avg_cr_predict_time / myStat->tt << endl;
    avg_writer << "    avg_cm_time: " << myStat->avg_cm_time / myStat->tt << endl;
    avg_writer << "      avg_canon_cm_time: " << myStat->avg_canon_cm_time / myStat->tt << endl;
    avg_writer << "      avg_decomp_cm_time: " << myStat->avg_decomp_cm_time / myStat->tt << endl;
    avg_writer << "      avg_match_time: " << myStat->avg_match_time / myStat->tt << endl;
    avg_writer << "        avg_isSubgraphOf2e_time: " << myStat->avg_isSubgraphOf2e_time / myStat->tt << endl;

    avg_writer << "  avg_client_time: " << myStat->avg_client_time / myStat->tt
               << endl;

    avg_writer << "avg_org_psb_map_cnt: " << myStat->avg_org_psb_map_cnt / myStat->tt << endl;
    avg_writer << "avg_red_psb_map_cnt: " << myStat->avg_red_psb_map_cnt / myStat->tt << endl;
    avg_writer << "avg_total_msg_size: "
               << myStat->avg_client_msg_size / myStat->tt << endl;
  }
};

#endif /* OUTPUTWRITER_H_ */
