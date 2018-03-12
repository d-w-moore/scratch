
split_path(*p,*tok,*col,*obj) {
    *col = trimr(*p, *tok)
    if(strlen(*col) == strlen(*p)) {
        *obj = *col
    } else {
        *obj = substr(*p, strlen(*col)+1, strlen(*p)) 
    }   
}

pep_database_reg_data_obj_post(*INST, *CTX, *OUT, *INFO) {
    writeLine("serverLog", "XXXX - pep_database_reg_data_obj_post");

    *src_lp = *INFO.logical_path
    *src_pp = *INFO.physical_path
    *src_rh = *INFO.resc_hier
    *src_sz = *INFO.data_size
    *src_ck = *INFO.checksum

    split_path(*src_lp, ".", *stem, *ext)
    if(*stem != *ext) {
        # apply file extension metadata
        writeLine("serverLog", "XXXX - apply irods::file_type metadata [*ext] for [*src_lp]")

        *meta = "irods::file_type=" ++ *ext
        writeLine("serverLog", "XXXX - apply irods::file_type meta [*meta]")
        msiString2KeyValPair(*meta, *meta_kvp);
        msiAssociateKeyValuePairsToObj(*meta_kvp, *src_lp, "-d")
    }

    msifile_stat(*src_lp, *src_pp, *src_rh, *stat_out)
    msiAssociateKeyValuePairsToObj(*stat_out, *src_lp, "-d")
}


pep_database_reg_replica_post(*INST, *CTX, *OUT, *SRC, *DST, *COND) {
    writeLine("serverLog", "XXXX - pep_database_reg_replica_post");

    *src_lp = *SRC.logical_path
    *src_pp = *SRC.physical_path
    *src_rh = *SRC.resc_hier
    *src_sz = *SRC.data_size
    *src_ck = *SRC.checksum

    *dst_lp = *DST.logical_path
    *dst_pp = *DST.physical_path
    *dst_rh = *DST.resc_hier
    *dst_sz = *DST.data_size
    *dst_ck = *DST.checksum
    *dst_rn = *DST.replica_number

    split_path(*dst_lp, ".", *stem, *ext)
    if(*stem != *ext) {
        *meta = "irods::file_type=" ++ *ext
        writeLine("serverLog", "XXXX - apply metadata [*meta]")
        msiString2KeyValPair(*meta, *meta_kvp);
        msiAssociateKeyValuePairsToObj(*meta_kvp, *src_lp, "-d")
    }

    msifile_stat(*dst_lp, *dst_pp, *dst_rh, *stat_out)
    msiAssociateKeyValuePairsToObj(*stat_out, *src_lp, "-d")
 
    *stat_sz = *stat_out.st_size
    if(*src_sz != *stat_sz) {
        writeLine("serverLog", "XXXX - size mismatch :: [*src_lp] needs update")
    }
}


