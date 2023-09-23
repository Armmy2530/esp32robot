//Calibration parameter
int dif_ref = 300;

void readSensor()
{
  sensor_val[0] = analogRead(L2_pin);
  sensor_val[1] = analogRead(L1_pin);
  sensor_val[2] = analogRead(C_pin);
  sensor_val[3] = analogRead(R1_pin);
  sensor_val[4] = analogRead(R2_pin);
  for (int n = 0; n <= 4; n++)
  {
    sensor_bool[n] = W(sensor_val[n], ref_sensor[n]);
  }
  L2_value = sensor_bool[0];
  L1_value = sensor_bool[1];
  C_value  = sensor_bool[2];
  R1_value = sensor_bool[3];
  R2_value = sensor_bool[4];
}

void calibrate_sensor(int n_check)
{
  readSensor();
  int white_val[5] = {0, 0, 0, 0, 0};
  int black_val[5] = {0, 0, 0, 0, 0};
  int avg_val[5] = {0, 0, 0, 0, 0};
  int confirm_black = 0;
  Serial.println("Start Calibrat sensor ..........");
  for (int i = 0; i < n_check; i++)
  {
    for (int n = 0; n <= 4; n++)
    {
      readSensor();
      white_val[n] = sensor_val[n];
    }
    m(150, 150);
    while (true)
    {
      readSensor();
      for (int n = 0; n <= 4; n++)
      {
        Serial.print(sensor_val[n]);
        Serial.print(" ");
      }
      Serial.println(" ");
      for (int i = 0; i <= 4; i++)
      {
        readSensor();
        if (abs(sensor_val[i] - white_val[i]) >= dif_ref)
        {
          confirm_black++;
        }
      }
      if (confirm_black == 5)
      {
        for (int n = 0; n <= 4; n++)
        {
          black_val[n] = sensor_val[n];
        }
        delay(50);
        m(0, 0);
        confirm_black = 0;
        break;
      }
      else
        confirm_black = 0;
    }
    for (int n = 0; n <= 4; n++)
    {
      if (avg_val[n] = 0)
      {
        avg_val[n] = (white_val[n] + black_val[n]) / 2;
      }
      else
      {
        avg_val[n] = avg_val[n] + ((white_val[n] + black_val[n]) / 2) / 2;
      }
    }
    Serial.print("loop: ");
    Serial.print(i);
    for (int n = 0; n <= 4; n++)
    {
      Serial.print(" ");
      Serial.print(avg_val[n]);
    }
    Serial.println();
    while (true)
    {
      readSensor();
      // for (int n = 0; n <= 4; n++)
      // {
      //     Serial.print(avg_val[n]);
      //     Serial.print(" ");
      //     Serial.print(sensor_val[n]);
      //     Serial.print(" ");
      //     Serial.print(W(sensor_val[n],avg_val[n]));
      //     Serial.print(" ");
      // }
      if ((W(sensor_val[0], avg_val[0]) && W(sensor_val[1], avg_val[1]) && W(sensor_val[2], avg_val[2]) && W(sensor_val[3], avg_val[3]) && W(sensor_val[4], avg_val[4])))
      {
        Serial.print("True ");
        break;
      }
      // Serial.println();
      m(-100, -100);
    }
    delay(500);
    m(0, 0);
    delay(1000);
  }
  for (int n = 0; n <= 4; n++)
  {
    ref_sensor[n] = avg_val[n];
    Serial.print("Final Ref");
    Serial.print(" ");
    Serial.print(ref_sensor[n]);
  }
  Serial.println();
}

void m(int l, int r)
{
  if (l > 255)
  {
    l = 255;
  }
  if (r > 255)
  {
    r = 255;
  }
  L_motor.m(l);
  R_motor.m(r);
}

bool W(int n, int ref)
{
  if (n > ref)
  {
    return true;
  }
  else
  {
    return false;
  }
}
bool B(int n, int ref)
{
  if (n <= ref)
  {
    return true;
  }
  else
  {
    return false;
  }
}
