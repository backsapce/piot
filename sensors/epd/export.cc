#include "../common/common.h"
#include "epd2in9.h"
#include "epdpaint.h"

namespace epd {
    #define COLORED      0
    #define UNCOLORED    1
    Epd epd;
    unsigned char* frame_buffer;
    Paint paint;
    uint8_t back_paint = 0;
    void frame_flush(uint16_t x , uint16_t y ){
        epd.SetFrameMemory(paint.GetImage(), x, y, paint.GetWidth(), paint.GetHeight());
        epd.DisplayFrame();
    }
    int8_t test_paint(){
        paint.DrawFilledRectangle(0, 10, 128, 34, COLORED);
        paint.DrawStringAt(0, 14, "Hello world!", &Font16, UNCOLORED);
        paint.DrawStringAt(0, 34, "e-Paper Demo", &Font16, COLORED);
        frame_flush(0,0);
        return 0;
    }

    int8_t paint_dht( char * c,  char * h, char * p){
        char buffer [20];
        if(back_paint == 0){
            paint.DrawStringAt(0, 14, "pi sensors data:", &Font16, COLORED);
            back_paint = 1;
            // change partial update
            if (epd.Init(lut_partial_update) != 0) {
                printf("e-Paper init failed\n");
                return -1;
            }
            frame_flush(0,0);
            frame_flush(0,0);
            paint.SetWidth(32);
            paint.SetHeight(25*11);
            return 0;
        }
        paint.Clear(UNCOLORED);
        sprintf (buffer, "C: %s H: %s P: %s",c, h, p);
        printf("%s\n",buffer);
        paint.DrawStringAt(0, 0, buffer, &Font16, COLORED);
        frame_flush(0,0);
        frame_flush(0,0);
        return 0;
    }

    napi_value Method(napi_env env, napi_callback_info info) {
        napi_status status;
        size_t argc = 3;
        napi_value args[3];
        status = napi_get_cb_info(env, info, &argc, args, nullptr, nullptr);
        assert(status == napi_ok);
        if (argc < 3) {
            napi_throw_type_error(env, nullptr, "Wrong number of arguments");
            return nullptr;
        }
         char *c = ( char*)malloc(10);
         char *h = ( char*)malloc(10);
         char *p = ( char*)malloc(10);
        status = napi_get_value_string_latin1(env, args[0], c,10,NULL);
        assert(status == napi_ok);
        status = napi_get_value_string_latin1(env, args[1], h,10,NULL);
        assert(status == napi_ok);
        status = napi_get_value_string_latin1(env, args[2], p,10,NULL);
        assert(status == napi_ok);
        paint_dht(c,h,p);
        free(c);
        free(h);
        // return void;
    }
    napi_value paint_wifi(napi_env env, napi_callback_info info){
        napi_status status;
        size_t argc = 1;
        napi_value args[1];
        status = napi_get_cb_info(env, info, &argc, args, nullptr, nullptr);
        assert(status == napi_ok);
        if (argc < 1) {
            napi_throw_type_error(env, nullptr, "Wrong number of arguments");
            return nullptr;
        }
        int32_t * flag;
        status = napi_get_value_int32(env,args[0],flag);
        assert(status == napi_ok);
        if(*flag == 1){
            paint.Clear(UNCOLORED);
            paint.DrawStringAt(50, 50, "NET OFF", &Font16, COLORED);
            frame_flush(0,0);
            frame_flush(0,0);
        }
    }
    napi_value  Init(napi_env env, napi_value exports) {
        napi_status status;
        napi_property_descriptor desc =
            {"paint", NULL, Method, NULL, NULL, NULL, napi_default, NULL};
        status = napi_define_properties(env, exports, 1, &desc);
        assert(status == napi_ok);
        if (epd.Init(lut_full_update) != 0) {
            printf("e-Paper init failed\n");
            napi_throw_type_error(env, "INIT_ERR", "epd init failed");
        }
        frame_buffer = (unsigned char*)malloc(epd.width / 8 * epd.height);
        paint.SetWidth(epd.width);
        paint.SetHeight(epd.height);
        paint.SetImage(frame_buffer);
        paint.Clear(UNCOLORED);
        frame_flush(0,0);
        frame_flush(0,0);
        return exports;
    }
    NAPI_MODULE(NODE_GYP_MODULE_NAME, Init)
}
