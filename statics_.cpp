#include "statics_.h"
#include "app_settings.h"

static app_settings settings = app_settings();

statics_::statics_()
{
}


statics_::~statics_()
{
}


app_settings & statics_::get_settings()
{
	return settings;
}
