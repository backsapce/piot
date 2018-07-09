#include "../common/common.h"
#include <sys/types.h>





namespace dht11 {

// #define PIN 3
#define MAXTIMINGS 85
#define MAX_TRY 5
static int DHTPIN = 4;
static int dht22_dat[5] = {0,0,0,0,0};

static uint8_t sizecvt(const int read)
{
  /* digitalRead() and friends from wiringpi are defined as returning a value
  < 256. However, they are returned as int() types. This is a safety function */

  if (read > 255 || read < 0)
  {
    printf("Invalid data from wiringPi library\n");
    exit(EXIT_FAILURE);
  }
  return (uint8_t)read;
}

static int read_dht22_dat()
{
  uint8_t laststate = HIGH;
  uint8_t counter = 0;
  uint8_t j = 0, i;

  dht22_dat[0] = dht22_dat[1] = dht22_dat[2] = dht22_dat[3] = dht22_dat[4] = 0;

  // pull pin down for 18 milliseconds
  pinMode(DHTPIN, OUTPUT);
  digitalWrite(DHTPIN, HIGH);
  delay(10);
  digitalWrite(DHTPIN, LOW);
  delay(18);
  // then pull it up for 40 microseconds
  digitalWrite(DHTPIN, HIGH);
  delayMicroseconds(40); 
  // prepare to read the pin
  pinMode(DHTPIN, INPUT);

  // detect change and read data
  for ( i=0; i< MAXTIMINGS; i++) {
    counter = 0;
    while (sizecvt(digitalRead(DHTPIN)) == laststate) {
      counter++;
      delayMicroseconds(2);
      if (counter == 255) {
        break;
      }
    }
    laststate = sizecvt(digitalRead(DHTPIN));

    if (counter == 255) break;

    // ignore first 3 transitions
    if ((i >= 4) && (i%2 == 0)) {
      // shove each bit into the storage bytes
      dht22_dat[j/8] <<= 1;
      if (counter > 16)
        dht22_dat[j/8] |= 1;
      j++;
    }
  }

  // check we read 40 bits (8bit x 5 ) + verify checksum in the last byte
  // print it out if data is good
  if ((j >= 40) && 
      (dht22_dat[4] == ((dht22_dat[0] + dht22_dat[1] + dht22_dat[2] + dht22_dat[3]) & 0xFF)) ) {
    return 1;
  }
  else
  {
    printf("Data not good, skip\n");
    return 0;
  }
}

napi_value Method(napi_env env, napi_callback_info info) {
  napi_status status;
  napi_value ret;
  napi_value nh;
  napi_value nt;
  int i = MAX_TRY;
  while(i-- > 0 && !read_dht22_dat());
  float t, h;
   h = (float)dht22_dat[0] * 256 + (float)dht22_dat[1];
	h /= 10;
    t = (float)(dht22_dat[2] & 0x7F)* 256 + (float)dht22_dat[3];
    t /= 10.0;

  status = napi_create_double(env, h, &nh);
  assert(status == napi_ok);
  status = napi_create_double(env, t, &nt);
  assert(status == napi_ok);
  status = napi_create_array_with_length(env, 2, &ret);
  assert(status == napi_ok);

  status = napi_set_property(env, ret,creat_array_index(env,0), nt);
  assert(status == napi_ok);
  status = napi_set_property(env, ret,creat_array_index(env,1), nh);
  assert(status == napi_ok);
  return ret;
}
napi_value  Init(napi_env env, napi_value exports) {
  napi_status status;
  napi_property_descriptor desc =
    {"data", NULL, Method, NULL, NULL, NULL, napi_default, NULL};
  status = napi_define_properties(env, exports, 1, &desc);
  assert(status == napi_ok);
  if (wiringPiSetupGpio () == -1)
    exit(EXIT_FAILURE) ;
	
  if (setuid(getuid()) < 0)
  {
    perror("Dropping privileges failed\n");
    exit(EXIT_FAILURE);
  }
  return exports;
}

	NAPI_MODULE(NODE_GYP_MODULE_NAME, Init)

}  // namespace dht11 end
