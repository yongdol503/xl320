#include <DynamixelWorkbench.h>

#if defined(__OPENCM904__)
  #define DEVICE_NAME "COM19" //Connected Port Name ex)ubuntu : ttyUSB0
#endif          

#define BAUDRATE  1000000

DynamixelWorkbench dxl_wb;
const char *Log = NULL;
bool result = false;
int dxl_id=1, i=0;
int dxl_new_id[6]={10,11,12,13,14,15};

void setup() 
{
  Serial.begin(57600);
  while(!Serial); // Wait for Opening Serial Monitor

  result = dxl_wb.init(DEVICE_NAME, BAUDRATE, &Log);
  if (result == false)
  {
    Serial.println(Log);
    Serial.println("Failed to init");
  }
  else
  {
    Serial.print("Succeeded to init : ");
    Serial.println(BAUDRATE);  
  }
}

void setting_all()
{
  
  Serial.println("setting all");
  result = dxl_wb.changeID(dxl_id, dxl_new_id[i%6], &Log);
  if (result == false)
  {
    Serial.print("changing ID:");
    Serial.println(dxl_new_id[i%6]);
  }
  else
  {
    Serial.println(Log);
    i++;
  }
}

void setting_one()
{
  int from_id, to_id, trash;
  Serial.println("setting one");
  while(!Serial.available()) {}
  from_id=Serial.parseInt();
  trash=Serial.read();
  while(!Serial.available()) {}
  to_id=Serial.parseInt();
  trash=Serial.read();
  
  result = dxl_wb.changeID(from_id, to_id, &Log);
  if (result == false)
  {
    Serial.println("input error");
    Serial.println(from_id);
    Serial.println(to_id);
  }
  else
  {
    Serial.println(Log);
  }
}

void loop() 
{
  char cmd='1',trash;
  uint8_t scanned_id[16];
  uint8_t dxl_cnt = 0;
  uint8_t range = 100;

  if(Serial.available())
  {
    cmd=Serial.read();
    trash=Serial.read();
  }
  Serial.println(cmd);
  if(cmd=='1') setting_all();
  else if(cmd=='2') setting_one();
  
  Serial.println("Wait for scan...");
  result = dxl_wb.scan(scanned_id, &dxl_cnt, range, &Log);
  if (result == false)
  {
    Serial.println(Log);
    Serial.println("Failed to scan");
  }
  else
  {
    Serial.print("Find ");
    Serial.print(dxl_cnt);
    Serial.println(" Dynamixels");

    for (int cnt = 0; cnt < dxl_cnt; cnt++)
    {
      Serial.print("id : ");
      Serial.print(scanned_id[cnt]);
      Serial.print(" model name : ");
      Serial.println(dxl_wb.getModelName(scanned_id[cnt]));
    }
  }
  delay(1000);
}
