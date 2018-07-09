#include "../common/common.h"
#include <wiringSerial.h>

namespace wzs{
const int MAX_IT = 20;
const int BAUD = 9600;
int port = -1;
napi_value Method(napi_env env, napi_callback_info info) {
  napi_status status;
  napi_value ret;
  int v = serialGetchar(port);
  int h;
  int l;
  int i = 0;
  while(v != 0xff && i < MAX_IT){
      i++;
      v = serialGetchar(port);
  }
    if(i>=MAX_IT){
       napi_throw_type_error(env, nullptr, "dart ws-z sensor error");
        return nullptr;
  }
  serialGetchar(port); //0x17
  serialGetchar(port); //0x04
  serialGetchar(port); //0x00
  h = serialGetchar(port); //h
  l = serialGetchar(port); //L
  serialGetchar(port); //0x07 full range h
  serialGetchar(port); //0xD0 full range l
  serialGetchar(port); //0x25
  int c = h * 256 + l;
    // printf("%d-%d\n",h,l);

  status = napi_create_int32(env, c, &ret);
  assert(status == napi_ok);
  return ret;
}

napi_value  Init(napi_env env, napi_value exports) {
  napi_status status;
  napi_property_descriptor desc =
    {"data", NULL, Method, NULL, NULL, NULL, napi_default, NULL};
  status = napi_define_properties(env, exports, 1, &desc);
  assert(status == napi_ok);

  port = serialOpen("/dev/ttyS0", BAUD);
  char beginCMD[] = {0xff,0x01,0x78,0x41,0x00,0x00,0x00,0x00,0x46};
  serialPuts(port, beginCMD);
  return exports;
}

NAPI_MODULE(NODE_GYP_MODULE_NAME, Init)

}