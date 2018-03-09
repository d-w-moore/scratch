// =-=-=-=-=-=-=-
// irods includes
#include "irods_error.hpp"
#include "irods_ms_plugin.hpp"

// =-=-=-=-=-=-=-
// stl includes
#include <string>
#include "boost/lexical_cast.hpp"

#include "fileStat.h"
#include "rcMisc.h"
#include "rsFileStat.hpp"
#include "irods_resource_manager.hpp"

extern irods::resource_manager resc_mgr;

int msifile_stat(
    msParam_t*      _logical_path,
    msParam_t*      _physical_path,
    msParam_t*      _resc_info,
    msParam_t*      _stat_out,
    ruleExecInfo_t* _rei ) {

    const char *logical_path = parseMspForStr( _logical_path );
    if( !logical_path ) {
        std::cout << "null _logical_path" << std::endl;
        return SYS_INVALID_INPUT_PARAM;
    }

    const char *physical_path = parseMspForStr( _physical_path );
    if( !physical_path ) {
        std::cout << "null _physical_path" << std::endl;
        return SYS_INVALID_INPUT_PARAM;
    }

    // _resc_info may be either a resc_id or a resc_hier
    rodsLong_t resc_id = 0; 
    if(strcmp(_resc_info->type, STR_MS_T ) != 0) { 
        resc_id = parseMspForPosInt(_resc_info);
        if( resc_id < 0 ) {
            std::cout << "invalid _resc_info" << std::endl;
            return SYS_INVALID_INPUT_PARAM;
        }
    }
    else {
        const char *resc_hier = parseMspForStr(_resc_info);
        if(!resc_hier) {
            std::cout << "null resc_hier" << std::endl;
            return SYS_INVALID_INPUT_PARAM;
        }

        irods::error err = resc_mgr.hier_to_leaf_id(resc_hier, resc_id);
        if(!err.ok()) {
            irods::log(PASS(err));
            return err.code();
        }
    }

    fileStatInp_t stat_inp;
    memset(&stat_inp, 0, sizeof(stat_inp));
    stat_inp.rescId = resc_id;
    strncpy(stat_inp.objPath,  logical_path,  sizeof(stat_inp.objPath));
    strncpy(stat_inp.fileName, physical_path, sizeof(stat_inp.fileName));

    rodsStat_t* stat_out = nullptr;
    int stat_err = rsFileStat(
                       _rei->rsComm,
                       &stat_inp,
                       &stat_out);
    if(stat_err < 0) {
        rodsLog(
            LOG_ERROR,
            "rsFileStat failed for [%s]  [%s] resc_id [%s]",
            logical_path,
            physical_path,
            resc_id);
        return stat_err;
    }

    try {
        keyValPair_t* kvp = (keyValPair_t*)malloc(sizeof(keyValPair_t));
        memset(kvp, 0, sizeof(keyValPair_t));

        std::string size = boost::lexical_cast<std::string>(stat_out->st_size);
        addKeyVal(kvp, "irods::st_size", size.c_str());

        std::string dev = boost::lexical_cast<std::string>(stat_out->st_dev);
        addKeyVal(kvp, "irods::st_dev", dev.c_str());

        std::string ino = boost::lexical_cast<std::string>(stat_out->st_ino);
        addKeyVal(kvp, "irods::st_ino", ino.c_str());

        std::string mode = boost::lexical_cast<std::string>(stat_out->st_mode);
        addKeyVal(kvp, "irods::st_mode", mode.c_str());

        std::string nlink = boost::lexical_cast<std::string>(stat_out->st_nlink);
        addKeyVal(kvp, "irods::st_nlink", nlink.c_str());

        std::string uid = boost::lexical_cast<std::string>(stat_out->st_uid);
        addKeyVal(kvp, "irods::st_uid", uid.c_str());

        std::string gid = boost::lexical_cast<std::string>(stat_out->st_gid);
        addKeyVal(kvp, "irods::st_gid", gid.c_str());

        std::string rdev = boost::lexical_cast<std::string>(stat_out->st_rdev);
        addKeyVal(kvp, "irods::st_rdev", rdev.c_str());

        std::string atim = boost::lexical_cast<std::string>(stat_out->st_atim);
        addKeyVal(kvp, "irods::st_atim", atim.c_str());

        std::string mtim = boost::lexical_cast<std::string>(stat_out->st_mtim);
        addKeyVal(kvp, "irods::st_mtim", mtim.c_str());

        std::string ctim = boost::lexical_cast<std::string>(stat_out->st_ctim);
        addKeyVal(kvp, "irods::st_ctim", ctim.c_str());

        std::string blksize = boost::lexical_cast<std::string>(stat_out->st_blksize);
        addKeyVal(kvp, "irods::st_blksize", blksize.c_str());

        std::string blocks = boost::lexical_cast<std::string>(stat_out->st_blocks);
        addKeyVal(kvp, "irods::st_blocks", blocks.c_str());

        _stat_out->inOutStruct = ( void * ) kvp;
        _stat_out->type = strdup( KeyValPair_MS_T );

    }
    catch(const boost::bad_lexical_cast& _e) {
        rodsLog(
            LOG_ERROR,
            "%s - lexical cast failed",
            __FUNCTION__);
        return INVALID_LEXICAL_CAST;
    }

    return 0;
}

extern "C"
irods::ms_table_entry* plugin_factory() {
    irods::ms_table_entry* msvc = new irods::ms_table_entry(4);
    msvc->add_operation<
        msParam_t*,
        msParam_t*,
        msParam_t*,
        msParam_t*,
        ruleExecInfo_t*>("msifile_stat",
                         std::function<int(
                             msParam_t*,
                             msParam_t*,
                             msParam_t*,
                             msParam_t*,
                             ruleExecInfo_t*)>(msifile_stat));
    return msvc;
}

