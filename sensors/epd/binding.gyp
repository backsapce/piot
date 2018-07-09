{
  "targets": [
    {
      "libraries":["-lwiringPi"],
      "target_name": "epd",
      "sources": [ "export.cc", "epd2in9.cc","epdif.cc","epdpaint.cc","./fonts/font8.cc" ,"./fonts/font16.cc" ,"./fonts/font20.cc" ,"./fonts/font24.cc" , "../common/common.cc" ]
    }
  ]
}