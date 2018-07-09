#include "common.h"

napi_value creat_array_index(napi_env env,uint8_t c)
{
    napi_value i;
    napi_status status;
    napi_create_uint32(env, c, &i);
    assert(status == napi_ok);
    return i;
}
