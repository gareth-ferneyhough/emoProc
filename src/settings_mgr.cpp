#include "settings_mgr.h"

SettingsMgr::SettingsMgr() :
    logger_(Logger::getInstance()),
    my_name_(typeid(*this).name())
{
    //ctor
}

SettingsMgr::~SettingsMgr()
{
    //dtor
}

SettingsMgr* SettingsMgr::getInstance()
{
    static SettingsMgr instance;
    return &instance;
}
